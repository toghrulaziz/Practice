#include <iostream>
#include <vector>


using namespace std;



// Bu kodlarda verilen tapshiriga elave olaraq diskin yazma sureti qeyd olunsun. ++
// (meselen : byte / saniye vahidi ile, deyek ki, yazilan faylin 10 byte olchusu var demeli fayl 1 saniyeye yazilacaq ++
//
//  bu emeliyyati hem createFile hem de setFileContent methodunda edin. ++
//
//  deleteFileByFilename(string filename) //eger file yoxdursa exception atsin, varsa file diskden silinsin  ++
//
//  updateFilename(string oldfilename, string newfilename) //eger fayl varsa adi deyishsin, yoxdursa exception atsin  ++
//
//  hashFile(string filename); //bu method fayli hash variante chevirir istenilen hash algorithminden istifade ede bilersiniz, --
//(internetden arashdirin) //bu method chagirilandan sonra, eger ichinde Hello World yazilmishdisa, artiq hash codelar gorunsun. --
//
// ELAVE OLARAQ : SSD KLASI YARADIN VE ARTIQ HDD ILE OLAN ORTAQ EMELIYYATLARI "DISC" KLASINA CHIXARIN ++
// HDD VE SSD DISKDEN YARANSIN; //SSD SURETI HDD DEN 10 DEFE CHOX OLSUN ++
//
//
// FILE = > CLASINA ELAVE OLARAQ Faylin YARANMA VAXTI, EN SON DEYISHILME VAXTI SAXLANILSIN ++
//
// DISC = > ShowAllFiles METHODU OLSUN HANSIKI BUTUN FAYLLAR GORUNSUN VE FAYLIN BUTUN MELUMATLARI GORUNSUN ++



class Exception : public exception {
	string text;
	int line;
	string source;
	string time;
public:

	Exception(string text, int line, string source, string time) {
		SetText(text);
		SetLine(line);
		SetSource(source);
		SetTime(time);
	}


#pragma region Setters
	void SetText(string text) {
		this->text = text;
	}

	void SetLine(int line) {
		this->line = line;
	}

	void SetSource(string source) {
		this->source = source;
	}

	void SetTime(string time) {
		this->time = time;
	}
#pragma endregion



#pragma region Getters
	string GetText()const { return text; }
	int GetLine()const { return line; }
	string GetSource()const { return source; }
	string GetTime()const { return time; }
#pragma endregion

};


class DatabaseException :public Exception {
public:
	DatabaseException(string text, int line, string source, string time) :Exception(text, line, source, time) {}
};



class InvalidArgumentException :public Exception {
public:
	InvalidArgumentException(string text, int line, string source, string time) :Exception(text, line, source, time) {}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////



class DISC {

public:
	class File {

	private:
		double fileSize;
		string fileName;
		string content;
		string createdTime;
		double speed;
	public:

		void SetFileSize() {
			this->fileSize = content.size();
		}

		void SetFileName(string fileName) {
			this->fileName = fileName;
		}

		void SetContent(string content) {
			this->content = content;
		}

		void SetCreatedTime(string createdTime) {
			this->createdTime = createdTime;
		}

		void SetSpeed(double speed) {
			this->speed = speed;
		}

		double GetFileSize() const { return fileSize; }
		string GetFileName()const { return fileName; }
		string GetContent()const { return content; }
		string GetCreatedTime()const { return createdTime; }
		double GetSpeed()const { return speed; }

		File(string fileName, string content, string createdTime = __TIME__) {
			SetFileName(fileName);
			SetContent(content);
			SetCreatedTime(createdTime);
		}

		File(string fileName, string content, string createdTime, double speed) : File(fileName, content, createdTime)
		{
			SetSpeed(speed);
		}



		friend ostream& operator<<(ostream& out, const File& o) {
			out << "File Name: " << o.fileName << "\nContent: " << o.content << "\nCreated Time: " << o.createdTime << "\nSpeed: " << o.speed << " byte / sec\n" << endl;
			return out;
		}

	};


public:
	vector<File> files;


	File& GetFile(string filename) { 
		if (files.empty()) {
			throw new DatabaseException("File is empty!", __LINE__, "GetFile()", __TIME__);
		}
		for (int i = 0; i < files.size(); i++) {
			if (files[i].GetFileName() == filename) {
				return files[i];
			}
			else
				throw new InvalidArgumentException("This Filename is not found!", __LINE__, "GetFile()", __TIME__);
		}
	}


	bool IsExistsFile(string filename) { 
		if (files.empty()) {
			throw new DatabaseException("File is empty!", __LINE__, "IsExistsFile()", __TIME__);
		}
		for (int i = 0; i < files.size(); i++) {
			if (files[i].GetFileName() == filename) {
				return true;
			}
			else
				return false;
		}
	}

	virtual void CreateFile(string filename, string content) = 0;

	virtual void SetFileContent(string filename, string content) = 0;


	void DeleteFileContent(string filename) { 
		for (int i = 0; i < files.size(); i++) {
			if (files[i].GetFileName() != filename) {
				throw new InvalidArgumentException("This Filename is not found!", __LINE__, "DeleteFileContent()", __TIME__);
			}
			if (files[i].GetFileName() == filename) {
				files[i].SetContent(" ");
				files[i].SetSpeed(0);
			}
		}
	}


	void UpdateFilename(string oldfilename, string newfilename) { 
		for (int i = 0; i < files.size(); i++) {
			if (files[i].GetFileName() != oldfilename) {
				throw new InvalidArgumentException("This Filename is not found!", __LINE__, "UpdateFilename()", __TIME__);
			}
			if (files[i].GetFileName() == oldfilename) {
				files[i].SetFileName(newfilename);
			}
		}
	}

	void DeleteFileByFilename(string filename) {
		for (int i = 0; i < files.size(); i++) {
			if (files[i].GetFileName() != filename) {
				throw new InvalidArgumentException("This Filename is not found!", __LINE__, "DelereFileByFilename()", __TIME__);
			}
			if (files[i].GetFileName() == filename) {
				files.erase(files.begin() + i);
				cout << "File Deleted!\n";
			}
		}
	}


	void ShowAllFile() {
		for (int i = 0; i < files.size(); i++) {
			cout << files[i];
		}
	}

};






class SSD : public DISC {
	double currentSize = 0;

	void SetCurrentSize(double currentSize) {
		this->currentSize = currentSize;
	}

	double GetCurrentSize()const { return currentSize; }

public:


	SSD(double currentSize)
	{
		SetCurrentSize(currentSize);
	}

	const static double maxSize;




	void CreateFile(string filename, string content) override { 
		double byte = content.size();
		double speed = byte;
		for (int i = 0; i < files.size(); i++) {
			if (files[i].GetFileName() == filename) {
				throw new DatabaseException("This Filename is not valid!", __LINE__, "CreateFile()", __TIME__);
			}
			if ((currentSize - files[i].GetFileSize() + content.size()) > maxSize) {
				throw new InvalidArgumentException("This content is not valid!", __LINE__, "CreateFile()", __TIME__);
			}
		}
		File newFile = File(filename, content, __TIME__, speed);
		files.push_back(newFile);
		currentSize += content.size();
	}



	void SetFileContent(string filename, string content) override { 
		double byte = content.size();
		double speed = byte;
		for (int i = 0; i < files.size(); i++) {
			if (files[i].GetFileName() != filename) {
				throw new InvalidArgumentException("This Filename is not valid!", __LINE__, "SetFileContent()", __TIME__);
			}
			if (files[i].GetFileName() == filename) {
				if (currentSize + content.size() > maxSize) {
					throw new InvalidArgumentException("This content is not valid!", __LINE__, "SetFileContent()", __TIME__);
				}
				files[i].SetContent(content);
				files[i].SetSpeed(speed);
				currentSize += content.size();
			}
		}
	}


};
const double SSD::maxSize = 1024;







class HDD : public DISC {
	double currentSize = 0;

	void SetCurrentSize(double currentSize) {
		this->currentSize = currentSize;
	}

	double GetCurrentSize()const { return currentSize; }

public:


	HDD(double currentSize)
	{
		SetCurrentSize(currentSize);
	}

	const static double maxSize;




	void CreateFile(string filename, string content) override { 
		double byte = content.size();
		double speed = byte / 10;
		for (int i = 0; i < files.size(); i++) {
			if (files[i].GetFileName() == filename) {
				throw new DatabaseException("This Filename is not valid!", __LINE__, "CreateFile()", __TIME__);
			}
			if ((currentSize - files[i].GetFileSize() + content.size()) > maxSize) {
				throw new InvalidArgumentException("This content is not valid!", __LINE__, "CreateFile()", __TIME__);
			}
		}
		File newFile = File(filename, content, __TIME__, speed);
		files.push_back(newFile);
		currentSize += content.size();
	}



	void SetFileContent(string filename, string content) override { 
		double byte = content.size();
		double speed = byte / 10;
		for (int i = 0; i < files.size(); i++) {
			if (files[i].GetFileName() != filename) {
				throw new InvalidArgumentException("This Filename is not valid!", __LINE__, "SetFileContent()", __TIME__);
			}
			if (files[i].GetFileName() == filename) {
				if (currentSize + content.size() > maxSize) {
					throw new InvalidArgumentException("This content is not valid!", __LINE__, "SetFileContent()", __TIME__);
				}
				files[i].SetContent(content);
				files[i].SetSpeed(speed);
				currentSize += content.size();
			}
		}
	}


};
const double HDD::maxSize = 1024;




class OperationSystem {
	OperationSystem() {}
public:
	static void controlPC();
};


void OperationSystem::controlPC() {
	DISC* hd = new HDD(1);
	DISC* ssd = new SSD(1);
	try
	{
		hd->CreateFile("newtext.txt", "hello world");
		ssd->CreateFile("SSD File 1", "hello world");
		hd->ShowAllFile();
		ssd->ShowAllFile();
	}
	catch (Exception* ex)
	{
		cout << "Text: " << ex->GetText() << "\nLine: " << ex->GetLine() << "\nSource: " << ex->GetSource() << "\nTime: " << ex->GetTime() << endl;
	}
}


void main() {
	OperationSystem::controlPC();
}