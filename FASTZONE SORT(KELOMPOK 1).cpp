#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <vector>
using namespace std;

#define MAX_TANGGAL 15

struct Lokasi {
    int kodePos;
    string namaLokasi;
};

vector<Lokasi> daftarLokasi = {
    {10510, "Cempaka Putih Timur"}, {10520, "Cempaka Putih Barat"},
    {10570, "Rawasari"}, {10110, "Gambir"}, {10120, "Kebon Kelapa"},
    {10130, "Petojo Utara"}, {10140, "Duri Pulo"}, {10150, "Cideng"},
    {10160, "Petojo Selatan"}, {10530, "Galur"}, {10540, "Tanah Tinggi"},
    {10550, "Kampung Rawa"}, {10560, "Johar Baru"}, {10610, "Gunung Sahari Selatan"},
    {10620, "Kemayoran"}, {10630, "Kebon Kosong"}, {10640, "Cempaka Baru"},
    {10650, "Utan Panjang"}, {10310, "Menteng"}, {10320, "Pegangsaan"},
    {10330, "Cikini"}, {10340, "Kebon Sirih"}, {10350, "Gondangdia"},
    {10710, "Pasar Baru"}, {10720, "Gunung Sahari Utara"}, {10730, "Mangga Dua Selatan"},
    {10740, "Karang Anyar"}, {10750, "Kartini"}, {10410, "Senen"},
    {10420, "Kwitang"}, {10430, "Kenari"}, {10440, "Paseban"},
    {10450, "Kramat"}, {10460, "Bungur"}, {10210, "Bendungan Hilir"},
    {10220, "Karet Tengsin"}, {10230, "Kebon Melati"}, {10240, "Kebon Kacang"},
    {10250, "Kampung Bali"}, {10260, "Petamburan"}, {10270, "Gelora"}
};

string cariLokasi(int kode) {
    for (const auto& lokasi : daftarLokasi) {
        if (lokasi.kodePos == kode)
            return lokasi.namaLokasi;
    }
    return "Tidak Diketahui";
}

struct Paket {
    int nomor;
    string namaBarang;
    string tanggalPesan;
    int kodePos;
    int jenisLayanan;
    int estimasiHari;
};

int konversiHari(int layanan) {
    switch (layanan) {
        case 1: return 0;
        case 2: return 1;
        case 3: return 4;
        case 4: return 5;
        case 5: return 14;
        default: return 99;
    }
}

string hitungTanggalSampai(const string& tanggal, int hariTambah) {
    int dd, mm, yyyy;
    sscanf(tanggal.c_str(), "%d/%d/%d", &dd, &mm, &yyyy);

    tm waktu = {};
    waktu.tm_mday = dd;
    waktu.tm_mon = mm - 1;
    waktu.tm_year = yyyy - 1900;

    time_t waktuAwal = mktime(&waktu);
    time_t waktuAkhir = waktuAwal + hariTambah * 86400;
    tm* hasil = localtime(&waktuAkhir);

    char buffer[MAX_TANGGAL];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", hasil);
    return string(buffer);
}

void inputPaket(vector<Paket>& daftar) {
    int n;
    cout << "Jumlah paket: ";
    cin >> n;
    cin.ignore();

    for (int i = 0; i < n; i++) {
        Paket p;
        p.nomor = i + 1;

        cout << "\nPaket " << p.nomor << ":\n";
        cout << "Nama Barang               : ";
        getline(cin, p.namaBarang);

        cout << "Tanggal Pesan (dd/mm/yyyy): ";
        getline(cin, p.tanggalPesan);

        cout << "Kode Pos                  : ";
        cin >> p.kodePos; cin.ignore();

        cout << "Pilih Jenis Layanan       :\n"
             << " 1. Same Day       (<24 jam)\n"
             << " 2. Next Day       (1 hari kerja)\n"
             << " 3. Reguler        (4 hari)\n"
             << " 4. Kargo          (5 hari)\n"
             << " 5. Ekspedisi Laut (14 hari)\n"
             << "Pilihan: ";
        cin >> p.jenisLayanan; cin.ignore();

        p.estimasiHari = konversiHari(p.jenisLayanan);
        daftar.push_back(p);
    }
}

void tukar(Paket& a, Paket& b) {
    Paket temp = a;
    a = b;
    b = temp;
}

int partition(vector<Paket>& arr, int low, int high) {
    Paket pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j].estimasiHari < pivot.estimasiHari ||
           (arr[j].estimasiHari == pivot.estimasiHari && arr[j].kodePos < pivot.kodePos)) {
            tukar(arr[++i], arr[j]);
        }
    }
    tukar(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<Paket>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void tampil(const vector<Paket>& daftar) {
    const string layanan[] = {"", "Same Day", "Next Day", "Reguler", "Kargo", "Ekspedisi Laut"};

    cout << "\n------------------------------------------------- Hasil Sorting ------------------------------------------------------\n";
    cout << "| No | Tgl Pesan   | Nama Barang          | Kode Pos  | Lokasi                | Jenis Layanan     |  Estimasi Sampai |\n";
    cout << "----------------------------------------------------------------------------------------------------------------------\n";

    for (const auto& p : daftar) {
        cout << "| " << setw(2) << p.nomor << " | "
             << setw(11) << p.tanggalPesan << " | "
             << setw(20) << p.namaBarang << " | "
             << setw(9) << p.kodePos << " | "
             << setw(21) << cariLokasi(p.kodePos) << " | "
             << setw(17) << layanan[p.jenisLayanan] << " | "
             << setw(16) << hitungTanggalSampai(p.tanggalPesan, p.estimasiHari) << " |\n";
    }

    cout << "----------------------------------------------------------------------------------------------------------------------\n";
}

int main() {
    vector<Paket> daftar;
    cout << "=== FASTZONE SORT (Estimasi Hari - C++) ===\n";
    inputPaket(daftar);
    quickSort(daftar, 0, daftar.size() - 1);
    tampil(daftar);
    return 0;
}

