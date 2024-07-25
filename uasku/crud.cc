#include <iostream>
#include <mysql/mysql.h>
#include <sstream>

using namespace std;

const char* hostname = "127.0.0.1";
const char* user = "root";
const char* pass = "123";
const char* dbname = "uas_crud";
unsigned int port = 31235;
const char* unixsocket = NULL;
unsigned long clientflag = 0;

MYSQL* connect_db() {
    MYSQL* conn = mysql_init(0);
    if (conn) {
        conn = mysql_real_connect(conn, hostname, user, pass, dbname, port, unixsocket, clientflag);
        if (conn) {
            cout << "Connected to the database successfully." << endl;
        } else {
            cerr << "Connection failed: " << mysql_error(conn) << endl;
        }
    } else {
        cerr << "mysql_init failed" << endl;
    }
    return conn;
}

void create_user(const string& username, const string& password, const string& role) {
    MYSQL* conn = connect_db();
    if (conn) {
        string query = "INSERT INTO users (username, password, role) VALUES ('" + username + "', '" + password + "', '" + role + "')";
        if (mysql_query(conn, query.c_str())) {
            cerr << "INSERT failed: " << mysql_error(conn) << endl;
        } else {
            cout << "User berhasil ditambahkan." << endl;
        }
        mysql_close(conn);
    }
}

bool login(const string& username, const string& password, string& role) {
    MYSQL* conn = connect_db();
    if (conn) {
        string query = "SELECT role FROM users WHERE username = '" + username + "' AND password = '" + password + "'";
        if (mysql_query(conn, query.c_str())) {
            cerr << "SELECT failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return false;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "mysql_store_result failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return false;
        }

        MYSQL_ROW row = mysql_fetch_row(res);
        if (row) {
            role = row[0];
            mysql_free_result(res);
            mysql_close(conn);
            return true;
        } else {
            mysql_free_result(res);
            mysql_close(conn);
            return false;
        }
    }
    return false;
}

void create_book(const string& name, const string& ketbuk, const string& penerbit, const string& penulis, const string& tahun_terbit) {
    MYSQL* conn = connect_db();
    if (conn) {
        string query = "INSERT INTO perpustakaan (name, keterangan_buku, penerbit, penulis, tahun_terbit) VALUES ('" + name + "', '" + ketbuk + "', '" + penerbit + "', '" + penulis + "', '" + tahun_terbit + "')";
        if (mysql_query(conn, query.c_str())) {
            cerr << "INSERT failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku berhasil ditambahkan." << endl;
        }
        mysql_close(conn);
    }
}

void get_books() {
    MYSQL* conn = connect_db();
    if (conn) {
        if (mysql_query(conn, "SELECT * FROM perpustakaan")) {
            cerr << "SELECT failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "mysql_store_result failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            cout << "ID: " << row[0] << ", Name: " << row[1] << ", Keterangan Buku: " << row[2] << ", Penerbit: " << row[3] << ", Penulis: " << row[4] << ", Tahun Terbit: " << row[5] << endl;
        }

        mysql_free_result(res);
        mysql_close(conn);
    }
}

void update_book(int book_id, const string& name, const string& ketbuk, const string& penerbit, const string& penulis, const string& tahun_terbit) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "UPDATE perpustakaan SET name = '" << name << "', keterangan_buku = '" << ketbuk << "', penerbit = '" << penerbit << "', penulis = '" << penulis << "', tahun_terbit = '" << tahun_terbit << "' WHERE id = " << book_id;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "UPDATE failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku berhasil diperbarui." << endl;
        }
        mysql_close(conn);
    }
}

void delete_book(int book_id) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "DELETE FROM perpustakaan WHERE id = " << book_id;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "DELETE failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku berhasil dihapus." << endl;
        }
        mysql_close(conn);
    }
}

void borrow_book(int book_id, const string& username) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "INSERT INTO pinjaman_buku (book_id, username) VALUES (" << book_id << ", '" << username << "')";
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "BORROW failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku berhasil dipinjam." << endl;
        }
        mysql_close(conn);
    }
}

void return_book(int book_id, const string& username) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "DELETE FROM pinjaman_buku WHERE book_id = " << book_id << " AND username = '" << username << "'";
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "RETURN failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku berhasil dikembalikan." << endl;
        }
        mysql_close(conn);
    }
}

int main() {
    int choice;
    string role;
    string username, password;

    while (true) {
        cout << "\nMenu Awal:\n";
        cout << "1. Registrasi\n";
        cout << "2. Login\n";
        cout << "3. Keluar\n";
        cout << "Masukan pilihan: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Masukan Username: ";
            cin >> username;
            cout << "Masukan Password: ";
            cin >> password;
            cout << "Masukan Role (admin/user): ";
            cin >> role;
            create_user(username, password, role);
        } else if (choice == 2) {
            cout << "Masukan Username: ";
            cin >> username;
            cout << "Masukan Password: ";
            cin >> password;
            if (login(username, password, role)) {
                cout << "Login berhasil sebagai " << role << endl;
                break;
            } else {
                cout << "Login gagal. Silakan coba lagi." << endl;
            }
        } else if (choice == 3) {
            return 0;
        } else {
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }
    }

    while (true) {
        cout << "\nMenu:\n";
        if (role == "admin") {
            cout << "1. Tambah Buku\n";
            cout << "2. Tampilkan Daftar Buku\n";
            cout << "3. Perbarui Buku\n";
            cout << "4. Hapus Buku\n";
            cout << "5. Keluar\n";
            cout << "Masukan pilihan: ";
            cin >> choice;

            if (choice == 1) {
                string name, ketbuk, penerbit, penulis, tahun_terbit;
                cout << "Masukan Judul Buku: ";
                cin.ignore();
                getline(cin, name);
                cout << "Masukan Keterangan Buku: ";
                getline(cin, ketbuk);
                cout << "Masukan Penerbit: ";
                getline(cin, penerbit);
                cout << "Masukan Penulis: ";
                getline(cin, penulis);
                cout << "Masukan Tahun Terbit: ";
                getline(cin, tahun_terbit);
                create_book(name, ketbuk, penerbit, penulis, tahun_terbit);
            } else if (choice == 2) {
                get_books();
            } else if (choice == 3) {
                int book_id;
                string name, ketbuk, penerbit, penulis, tahun_terbit;
                cout << "Masukan ID Buku Yang akan diedit: ";
                cin >> book_id;
                cin.ignore();
                cout << "Masukan Judul Baru Buku: ";
                getline(cin, name);
                cout << "Masukan Keterangan Buku Baru: ";
                getline(cin, ketbuk);
                cout << "Masukan Penerbit Baru: ";
                getline(cin, penerbit);
                cout << "Masukan Penulis Baru: ";
                getline(cin, penulis);
                cout << "Masukan Tahun Terbit Baru: ";
                getline(cin, tahun_terbit);
                update_book(book_id, name, ketbuk, penerbit, penulis, tahun_terbit);
            } else if (choice == 4) {
                int book_id;
                cout << "Masukan ID Buku Untuk dihapus: ";
                cin >> book_id;
                delete_book(book_id);
            } else if (choice == 5) {
                break;
            } else {
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
            }
        } else if (role == "user") {
            cout << "1. Tampilkan Daftar Buku\n";
            cout << "2. Pinjam Buku\n";
            cout << "3. Kembalikan Buku\n";
            cout << "4. Keluar\n";
            cout << "Masukan pilihan: ";
            cin >> choice;

            if (choice == 1) {
                get_books();
            } else if (choice == 2) {
                int book_id;
                cout << "Masukan ID Buku Yang ingin dipinjam: ";
                cin >> book_id;
                borrow_book(book_id, username);
            } else if (choice == 3) {
                int book_id;
                cout << "Masukan ID Buku Yang ingin dikembalikan: ";
                cin >> book_id;
                return_book(book_id, username);
            } else if (choice == 4) {
                break;
            } else {
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
            }
        }
    }

    return 0;
}
