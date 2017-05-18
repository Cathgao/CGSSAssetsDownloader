#include "CGSSMusicDownloader.h"

using namespace std;

string s;
string update_list = "";
string res_ver;
const char* cmd;

static int get_b(void *data, int argc, char **argv, char **azColName) {
	string name(argv[0]);
	name = name.substr(2, name.length() - 6);
	
	fstream _file;
	string fileName = "b\\" + name + ".wav";
	_file.open(fileName.c_str(), ios::in);

	if (!_file) {
		update_list += name;
		update_list += "\n";
		printf(name.c_str());
		printf("\n");
		s += "http://storage.game.starlight-stage.jp/dl/resources/High/Sound/Common/b/";
		s += argv[1];
		s += "\n";
	}
	else {
		s += "";
	}
	_file.close();
	return 0;
}

static int get_l(void *data, int argc, char **argv, char **azColName) {
	string name(argv[0]);
	name = name.substr(2, name.length() - 6);

	fstream _file;
	string fileName = "l\\" + name + ".wav";
	_file.open(fileName.c_str(), ios::in);

	if (!_file) {
		update_list += name;
		update_list += "\n";
		printf(name.c_str());
		printf("\n");
		s += "http://storage.game.starlight-stage.jp/dl/resources/High/Sound/Common/l/";
		s += argv[1];
		s += "\n";
	}
	else {
		s += "";
	}
	_file.close();

	return 0;
}

long getFileSize2(const char* strFileName)
{
	struct _stat info;
	_stat(strFileName, &info);
	long size = info.st_size;
	return size;
}

void download_manifest(string resource_version) {
	string url, ssd;
	system("md data");
	url = "tools\\wget\\wget -c -O data\\manifest_" + resource_version + " http://storage.game.starlight-stage.jp/dl/" + res_ver + "/manifests/Android_AHigh_SHigh";
	cmd = url.c_str();
	system(cmd);

	string lz4file = "data\\manifest_" + resource_version;
	long size = getFileSize2(lz4file.c_str());
	if (size < 1) {
		printf("����ʧ�ܡ�\n");
		string remove;
		remove = "del data\\manifest_" + resource_version + " /f /s /q";
		system(remove.c_str());
		exit(0);
	}
	
	ssd = "tools\\SSDecompress\\SSDecompress.exe data\\manifest_" + resource_version;
	cmd = ssd.c_str();
	system(cmd);

	system("del data\\*.");
	system("ren data\\*.unity3d *.db");
}

void create_download_list(string resource_version) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	const char* data = "Callback function called";

	string sqlfile = "data\\manifest_" + resource_version + ".db";
	rc = sqlite3_open(sqlfile.c_str(), &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else {
		fprintf(stderr, "�ɹ������ݿ⡣\n");
	}

	s = "";
	sql = "SELECT * FROM manifests WHERE name LIKE 'b/%acb'";
	rc = sqlite3_exec(db, sql, get_b, (void*)data, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		exit(0);
	}
	else {
		ofstream b_txt;
		b_txt.open("data\\b.txt");
		b_txt << s;
		b_txt.close();
		fprintf(stdout, "�ɹ�����bgm�����б�\n");
	}

	s = "";
	sql = "SELECT * FROM manifests WHERE name LIKE 'l/%acb'";
	rc = sqlite3_exec(db, sql, get_l, (void*)data, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		exit(0);
	}
	else {
		ofstream l_txt;
		l_txt.open("data\\l.txt");
		l_txt << s;
		l_txt.close();
		fprintf(stdout, "�ɹ�����live�����б�\n");		
	}

	sqlite3_close(db);
}

void download_music() {
	fstream file;
	int ch;

	file.open("data\\b.txt", ios::in);
	ch = file.get(); //��ͼȥ��һ���ַ�
	if (!file.eof()) {//�� if ( ch==EOF )  //һ���ַ���δ��������ʾ�ļ�Ϊ��
		system("tools\\wget\\wget -c -P ./b/acb -i data\\b.txt");
		system("for %i in (b\\acb\\*.) do tools\\DereTore.ACB\\DereTore.ACB.Test.exe %i");
		system("md b\\hca");
		system("for /f \"delims=\" %f in ('dir /s /b b\\*.hca') do move \"%f\" b\\hca\\");
		system("rd b\\acb /s /q");
		system("for %i in (b\\hca\\*.hca) do tools\\HCADecoder\\hca.exe -v 1.0 -m 16 -l 0 -a F27E3B22 -b 00003657 %i");
		system("for /f \"delims=\" %f in ('dir /s /b b\\hca\\*.wav') do move \"%f\" b\\");
		system("rd b\\hca /s /q");
	}
	else {
		printf("bgm������¡�\n");
	}
	file.close();

	file.open("data\\l.txt", ios::in);
	ch = file.get(); //��ͼȥ��һ���ַ�
	if (!file.eof()) {//�� if ( ch==EOF )  //һ���ַ���δ��������ʾ�ļ�Ϊ��
		system("tools\\wget\\wget -c -P ./l/acb -i data\\l.txt");
		system("for %i in (l\\acb\\*.) do tools\\DereTore.ACB\\DereTore.ACB.Test.exe %i");
		system("md l\\hca");
		system("for /f \"delims=\" %f in ('dir /s /b l\\*.hca') do move \"%f\" l\\hca\\");
		system("rd l\\acb /s /q");
		system("for %i in (l\\hca\\*.hca) do tools\\HCADecoder\\hca.exe -v 1.0 -m 16 -l 0 -a F27E3B22 -b 00003657 %i");
		system("for /f \"delims=\" %f in ('dir /s /b l\\hca\\*.wav') do move \"%f\" l\\");
		system("rd l\\hca /s /q");
	}
	else {
		printf("live������¡�\n");
	}
	file.close();

	printf("\n======================================\n\n");
	cout << update_list;
	printf("���³ɹ���\n\n======================================\n");
}

int main(int argc, char* argv[])
{
	if (argc == 1 || argc > 2) {
		printf("��ӭʹ��CGSSMusicDownloader ver.1.0\n\n");
		printf("�÷�: CGSSMusicDownloader <resource_version>\n");
		printf("����: CGSSMusicDownloader 10026450\n");
		printf("����㲻֪�����ݿ�汾�ţ������\nhttps://starlight.kirara.ca/api/v1/info \n�鿴���°汾��\n");
		printf("\n");
		printf("��ϵ���ߣ�����@���y_Nyanko, ΢��@TTPTs\n\n");
		system("pause");
		exit(0);
	}
	res_ver = argv[1];

	fstream _file;
	string fileName = "data\\manifest_" + res_ver + ".db";
	_file.open(fileName.c_str(), ios::in);

	if (!_file) {
		printf("���ݿ��ļ������ڣ���ʼ�������ݿ⡣\n");
		download_manifest(res_ver);
	}
	else {
		printf("�ð汾���ݿ��ļ��Ѵ��ڣ���ʼ������Դ�ļ���\n");
	}
	create_download_list(res_ver);
	download_music();

	return 0;
}

