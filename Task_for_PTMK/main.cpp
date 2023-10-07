#include "stdafx.h"

//установка адреса и порта сервера (в данном случае локального серера)
const std::string server = "tcp://127.0.0.1:3306";
const std::string username = "root"; //имя пользователя БД
const std::string password = "root"; //пароль пользователя БД

int id_emp = 0;

//Обновление счетчика id сотрудников
void setIdEmployee(sql::Connection*&con) {
	sql::PreparedStatement* pstmt;
	sql::ResultSet* result;
	pstmt = con->prepareStatement("SELECT MAX(idemployees) FROM employees;");
	result = pstmt->executeQuery();
	while(result->next())
		id_emp = result->getInt(1) + 1;
}

class Employee {
public:

	void setEmployee(const std::string emp_surname, std::string emp_name, std::string emp_patronymic, const std::string emp_birthday, const std::string emp_gender) {
		this->emp_surname = emp_surname;
		this->emp_name = emp_name;
		this->emp_patronymic = emp_patronymic;
		this->emp_birthday = emp_birthday;
		this->emp_gender = emp_gender;
	}

	void sendEmpInfo(sql::Connection*& con) {
		setIdEmployee(con);
		sql::PreparedStatement* pstmt;
		pstmt = con->prepareStatement("INSERT INTO employees (idemployees, emp_surname, emp_name, emp_patronymic, emp_birthday, emp_gender) VALUES(?,?,?,?,?,?);");
		pstmt->setInt(1, id_emp);
		pstmt->setString(2, this->emp_surname);
		pstmt->setString(3, this->emp_name);
		pstmt->setString(4, this->emp_patronymic);
		pstmt->setDateTime(5, this->emp_birthday);
		pstmt->setString(6, this->emp_gender);
		pstmt->execute();
		delete pstmt;
		std::cout << "Информация о сотруднике отправлена\n";
	}


	int getAge(std::string emp_birthday) {
		int age = 0, year, month, day;
		std::string temp_emp_birthday, temp_year, temp_month, temp_day;
		for (char c : emp_birthday) {
			if (c != '-') {
				this->emp_birthday += c;
			}
		}

		for (int i = 0; i < this->emp_birthday.length(); i++) {
			i < 4 ? temp_year += this->emp_birthday[i] : (i >= 4 && i <= 5 ? temp_month += this->emp_birthday[i] : temp_day += this->emp_birthday[i]);
		}
		this->emp_birthday = "";

		year = stoi(temp_year);
		month = stoi(temp_month);
		day = stoi(temp_day);

		std::tm birth_tm{};
		birth_tm.tm_year = year - 1900;
		birth_tm.tm_mon = month - 1;
		birth_tm.tm_mday = day;

		std::time_t t = std::time(nullptr) ;
		std::tm* date_now = std::localtime(&t);

		birth_tm.tm_mon <= date_now->tm_mon && birth_tm.tm_mday <= date_now->tm_mday ? age = date_now->tm_year - birth_tm.tm_year : age = date_now->tm_year - birth_tm.tm_year - 1;
		return age;
	}

	void insertHundredMaleF(sql::Connection*& con) {
		sql::PreparedStatement* pstmt;
		char F = 'F';
		pstmt = con->prepareStatement("INSERT INTO employees VALUES(?,?,?,?,?,?)");
		for (int i = 0; i < 100; i++)
		{
			setIdEmployee(con);
			generateEmpInfo(F);
			pstmt->setInt(1,id_emp);
			pstmt->setString(2, this->emp_surname);
			pstmt->setString(3, this->emp_name);
			pstmt->setString(4, this->emp_patronymic);
			pstmt->setDateTime(5, this->emp_birthday);
			pstmt->setString(6, this->emp_gender);
			pstmt->execute();
		}
		delete pstmt;
		std::cout << "Добавлено 100 новых сотрудников мужского пола с фамилией начинающийся с 'F'!\n";
	}

	//Вставка информации о сотрудниках с помощью файла
	//void insertThousandEmps(sql::Connection*& con) {
	//	sql::Statement* stmt;
	//	char F = 'E';
	//	std::ofstream fout;
	//	std::string fpath = "InsertData.txt";
	//	int SIZE = 1000000;
	//
	//	for (int i = 1; i <= SIZE; i++)
	//	{
	//		setIdEmployee(con);
	//		generateEmpInfo(F);
	//
	//		try
	//		{
	//			fout.open(fpath, std::ofstream::app);
	//			fout << id_emp << "\t" << this->emp_surname << "\t" << this->emp_name << "\t" << emp_patronymic << "\t" << this->emp_birthday << " \t" << this->emp_gender << "\n";
	//			if (i % 100 == 0 ) {
	//				stmt = con->createStatement();
	//				stmt->execute("LOAD DATA LOCAL INFILE 'C:/Users/denxa/source/repos/Task_for_PTMK/" + fpath + "' INTO TABLE employees");
	//				delete stmt;
	//				fout.clear();
	//			}
	//		}
	//		catch (const std::exception&ex)
	//		{
	//			std::cout << "Error: " << ex.what() << "\n";
	//		}
	//		fout.close();
	//	}
	//	std::cout << "Добавлено 1000000 новых сотрудников!\n";
	//}

private:
	std::string emp_name, emp_surname, emp_patronymic, emp_birthday, emp_gender;

	//Генерирование информации о сотрудниках
	void generateEmpInfo(char maleF) {
		int id_gender = rand() % 2 + 1;
		int id_name = rand() % 8 + 1;
		int id_surename = rand() % 12 + 1;
		int id_patronymic = rand() % 8 + 1;

		std::map<int, std::string> map_male_name = { {1,"Ivan"},{2,"Aleksandr"},{3,"Mikhail"},
													{4,"Aleksey"},{5,"Evgeniy"},{6, "Andrey"},
													{7,"Kirill"},{8,"Anatoliy"}};

		std::map<int, std::string> map_male_surname_F = { {1,"Faustov"},{2,"Fateev"},{3,"Fokin"},
														{4,"Florov"},{5,"Flyagin"},{6, "Frumin"},
														{7,"Furin"},{8,"Fryakov"},{9,"Fonin"},
														{10,"Fomin"},{11,"Filonov"},{12,"Filipkov"}};

		std::map<int, std::string> map_male_surname = { {1,"Ivanov"},{2,"Fedonin"},{3,"Smirnov"},
														{4,"Sobolev"},{5,"Gagarin"},{6, "Kuznetsov"},
														{7,"Kozlov"},{8,"Reznik"},{9,"Zimin"},
														{10,"Shishov"},{11,"Popov"},{12,"Baburin"} };

		std::map<int, std::string> map_male_patronymic = { {1,"Aleksandrovich"},{2,"Sergeevich"},{3,"Ivanovich"},
															{4,"Nikitich"},{5,"Igorevich"},{6,"Lada"},
															{7,"Dmitrievich"},{8,"Olegovich"} };
		
		std::map<int, std::string> map_female_name = { {1,"Anastasiya"},{2,"Zhanna"},{3,"Kira"},
														{4,"Natal'ya"},{5,"Tat'yana"},{6,"Lada"},
														{7,"Vera"},{8,"Irina"} };

		std::map<int, std::string> map_female_surname = { {1,"Ivanova"},{2,"Fedonina"},{3,"Smirnova"},
														{4,"Gluhova"},{5,"Bobrova"},{6, "Kuznetsova"},
														{7,"Kozlova"},{8,"Avdeeva"},{9,"Dudkina"},
														{10,"Astahova"},{11,"Popova"},{12,"Grekova"} };

		std::map<int, std::string> map_female_patronymic = { {1,"Aleksandrovna"},{2,"Sergeevna"},{3,"Ivanova"},
															{4,"Nikitovna"},{5,"Igorevna"},{6,"Ahmatova"},
															{7,"Dmitrievna"},{8,"Olegovna"} };
		
		if (maleF == 'F') {
			setEmployee(map_male_surname_F[id_surename], map_male_name[id_name], map_male_patronymic[id_patronymic], "2000-01-01", "Male");
		}
		
		else if (id_gender == 1 && maleF != 'F') {
			setEmployee(map_male_surname[id_surename], map_male_name[id_name], map_male_patronymic[id_patronymic], "2000-01-01", "Male");
		}

		else {
			setEmployee(map_female_surname[id_surename], map_female_name[id_name], map_female_patronymic[id_patronymic], "2000-01-01", "Female");
		}

	}
};

class TableEmloyees {
public:

	void createTable(sql::Connection*& con) {
		sql::Statement* stmt;
		stmt = con->createStatement();
		
		stmt->execute("DROP TABLE IF EXISTS employees;");
		std::cout << "Удаление таблицы employees (if existed) завершено" << "\n";
		
		stmt->execute("CREATE TABLE employees (idemployees INT PRIMARY KEY, emp_name VARCHAR(25), emp_surname VARCHAR(25), emp_patronymic VARCHAR(25), emp_birthday DATE, emp_gender VARCHAR(10));");
		delete stmt;
		std::cout << "Создание таблицы employees завершено"<< "\n";
		
	}

	void selectEmpInfo(sql::Connection*& con) {
		Employee emp;
		sql::PreparedStatement* pstmt;
		sql::ResultSet* result;
		pstmt = con->prepareStatement("SELECT * FROM employees");
		result = pstmt->executeQuery();
		while (result->next())
		{
			std::printf("Полное имя: %s %s %s, дата рождения: %s, пол: %s, полных лет: %d\n", result->getString(3), result->getString(2), result->getString(4), result->getString(5), result->getString(6), emp.getAge(result->getString(5)));
		}
		delete pstmt;
		delete result;
	}

	void selectEmpMaleF(sql::Connection*& con) {
		sql::PreparedStatement* pstmt;
		sql::ResultSet* result;
		pstmt = con->prepareStatement("SELECT * FROM employees WHERE (employees.emp_gender = 'male' or employees.emp_gender = 'Male') AND employees.emp_surname LIKE 'F%';");
		result = pstmt->executeQuery();

		while (result->next())
		{
			std::printf("Полное имя сотрудника: %s %s %s\n", result->getString(3), result->getString(2), result->getString(4));
		}
		delete pstmt;
		delete result;
	}

private:

};


int main()
{
	setlocale(LC_ALL, "RU");
	std::srand(time(nullptr));

	sql::Driver* driver;
	sql::Connection* con;
	sql::Statement* stmt;

	try
	{
		driver = get_driver_instance();
		con = driver->connect(server, username, password);
	}
	catch (sql::SQLException e)
	{
		std::cout << "Could not connect to server. Error message: " << e.what() << "\n";
		system("pause");
		exit(1);
	}

	//установка схемы базы данных
	con->setSchema("task_for_ptmk_db");

	stmt = con->createStatement();
	stmt->execute("SET GLOBAL local_infile=1;");

	int action = 0;

	while (action != 6) {

		std::cout << "Выберите действие:\n1 - Создание таблицы с полями справочника сотрудников;\n2 - Создание записи справочника сотрудников;\n3 - Вывод всех строк справочника сотрудников, с уникальным значением ФИО+дата, отсортированным по ФИО;\n4 - Заполнение автоматически 1000000 строк справочника сотрудников;\n5 - Результат выборки из таблицы по критерию: пол мужской, Фамилия начинается с 'F'.\n6 - Выход\n";
		std::cin >> action;

		if (action == 1) {
			TableEmloyees emps;
			try
			{
				emps.createTable(con);
			}
			catch (const sql::SQLException e)
			{
				std::cout << e.what() << "\n";
			}
			
		}
		
		else if (action == 2) {
			Employee emp;
			std::string emp_name, emp_surname, emp_patronymic, emp_birthday, emp_gender;
			
			std::cout << "Введите ФИО сотрудника (кириллица), дату рождения (гггг-мм-дд) и пол (male | female):\n";
			std::cin >> emp_surname >> emp_name >> emp_patronymic >> emp_birthday >> emp_gender;
			emp.setEmployee(emp_surname, emp_name, emp_patronymic, emp_birthday, emp_gender);
			emp.sendEmpInfo(con);
		}

		else if (action == 3) {
			TableEmloyees emps;
			emps.selectEmpInfo(con);
		}

		else if (action == 4) {
			Employee emp;
			try
			{
				//Вставка 1000000 сотрудников
				//emp.insertThousandEmps(con);
			}
			catch (const sql::SQLException&ex)
			{
				std::cout << "Error: " << ex.what() << "\n";
			}
		}

		else if (action == 5) {
			try
			{
				TableEmloyees t_emps;
				t_emps.selectEmpMaleF(con);
			}
			catch (const sql::SQLException&ex)
			{
				std::cout << "Error: " << ex.what() << "\n";
			}

		}

		else if (action == 6) {
			return 0;
		}

		else {
			std::cout << "Данной команды не существует!\n";
		}
	}

	delete con;
	system("pause");
	return 0;
}