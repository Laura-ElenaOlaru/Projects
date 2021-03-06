#pragma once
#include "Exceptii.h"
#include "Film.h"
#include "VectorDinamic.h"
#include <vector>
using std::vector;
#include <numeric>
//using std::iota;

#include <memory>
#include<functional>
#include <algorithm>
using std::transform;

#include <unordered_map>
using std::unordered_map;

#include <random> // std::default_random_engine
#include <chrono> // std::chrono::system_clock


class RepositoryMare
{
public:
	RepositoryMare() = default;

	virtual void initializeazaRepo() = 0;
	virtual vector<Film> getCos() = 0;
	virtual Film getFilmCos(Film film) = 0;
	virtual void adaugaCos(string titlu) = 0;
	virtual void golesteCos() = 0;
	virtual void genereazaCos(int nrFilme) = 0;
	virtual string getPozitie(const Film& film) = 0;
	virtual void addFilm(const Film& film) = 0;
	virtual void removeFilm(const Film& film) = 0;
	virtual void updateGen(Film& film, string genC) = 0;
	virtual void updateActor(Film& film, string actorC) = 0;
	virtual void updateAn(Film& film, int anC) = 0;
	virtual vector<Film> getAll() = 0;
	virtual unordered_map<string, Film>::iterator getFilm(string titlu) = 0;
	virtual int getRepoLungime() = 0;
};

class Repository :public RepositoryMare {
protected:
	vector<Film> cos;
	unordered_map<string, Film> filme;

public:
	Repository() = default; //default constructor

	/*Initializeaza repository cu 10 filme*/
	void initializeazaRepo();

	/*Returneaza lista cos*/
	vector<Film> getCos();

	Film getFilmCos(Film film);

	/*Adauga un film in lista cos, gasit dupa titlu in repository
	Arunca exceptie daca filmul e inexistent*/
	void adaugaCos(string titlu);

	/*Goleste lista cos de toate filmele*/
	void golesteCos();

	/*Genereaza random nrFilme filme in cos*/
	void genereazaCos(int nrFilme);


	/*Returneaza pozitia filmului film*/
	string getPozitie(const Film& film);

	/*Adauga filmul in lista de filme
	Arunca exceptie daca filmul deja exista*/
	virtual void addFilm(const Film& film);

	/*Sterge filmul din lista de filme
	Arunca exceptie daca filmul nu exista*/
	virtual void removeFilm(const Film& film);

	/*Modifica titlul filmului din lista de filme cu titluC*/
	//virtual void updateTitlu(Film& film, string& titluC);

	/*Modifica genul filmului din lista de filme cu genC*/
	virtual void updateGen(Film& film, string genC);

	/*Modifica actorul filmului din lista de filme cu actorC*/
	virtual void updateActor(Film& film, string actorC);

	/*Modifica anul filmului din lista de filme cu anC*/
	virtual void updateAn(Film& film, int anC);

	/*Returneaza lista de filme*/
	vector<Film> getAll();

	/*Rrturneaza filmul de pe pozitia i*/
	unordered_map<string, Film>::iterator getFilm(string titlu);

	/*Retuneaza lungime repository*/
	int getRepoLungime();
};

class RepositoryFile :public Repository {
private:
	float probabilitate;
	string fileName;
	void loadFromFile();
	void writeToFile();
public:
	RepositoryFile() = default;
	RepositoryFile(string fileName, float probabilitate) :Repository(), fileName{ fileName }, probabilitate{ probabilitate }{
		loadFromFile(); // in constructor incarcam datele din fisier
	}
	void addFilm(const Film& film) override {
		try {
			Repository::addFilm(film); // apelam metoda din clasa de baza
			writeToFile();
		}
		catch (RepoException& ex) {
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			cout << r << '\n';
			if (r > probabilitate)
				throw ex;
		}
	}
	void removeFilm(const Film& film) override {
		try {
			Repository::removeFilm(film);
			writeToFile();
		}
		catch (RepoException& ex) {
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			cout << r << '\n';
			if (r > probabilitate)
				throw ex;
		}
	}
	void updateGen(Film& film, string genC) override {
		Repository::updateGen(film, genC);
		writeToFile();
	}
	void updateActor(Film& film, string actorC) override {
		Repository::updateActor(film, actorC);
		writeToFile();
	}
	void updateAn(Film& film, int anC) override {
		Repository::updateAn(film, anC);
		writeToFile();
	}
	void setFileName(string fileName);
	string getFileName();
};