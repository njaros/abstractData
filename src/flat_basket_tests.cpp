# include "../includes/flat_basket.hpp"
# include "../includes/map.hpp"
# include <iostream>
# include <fstream>
# include <string>

template < typename T >
void    displayDataMap(T& container, const char* name, std::ostream& outfile)
{
	typename T::iterator     it;

	it = container.begin();
	outfile << name << " content :" << '\n';
	while (it != container.end())
	{
		outfile << (*it).first << " => " << (*it).second << '\n';
		it++;
	}
	outfile << '\n';
}

template < typename T >
void    displayContent(const T& container, const char* name, std::ostream& outfile)
{
	typename T::const_iterator it;
	typename T::const_iterator end;

	it = container.begin();
	end = container.end();
	outfile << "Content";
	if (name)
		outfile << " of " << name;
	outfile << " :\n";
	while (it != end)
	{
		outfile << *it << '\n';
		it++;
	}
	outfile << '\n';
}

template <class T>
static void	displayData(const ft::flat_basket<T>& f, bool displayGround, const char* name, std::ostream& outfile)
{
	outfile << "Data";
	if (name)
		outfile << " of " << name;
	outfile << " :\n";
	outfile << "size = " << f.size();
	outfile << "\ncapacity = " << f.capacity();
	outfile << "\nsize left = " << f.size_left();
	outfile << "\nmax size = " << f.max_size();
	outfile << "\nground ptr = " << f.ground();
	outfile << "\nmanage his own ground = " << f.manage_ground();
	outfile << '\n';
	displayContent(f, name, outfile);
	if (displayGround)
		displayContent(*f.ground(), "ground", outfile);
	outfile << '\n';
}

void	flat_basket_tests(const std::string& currentPath)
{
	typedef ft::flat_basket<std::string>	S;
	typedef ft::flat_basket<int>			I;

	std::string fileName;
	std::ofstream outfile;
	std::boolalpha(outfile);

	ft::vector<std::string> ground;
	ground.push_back("pouet");
	ground.push_back("pouet2");
	ground.push_back("pouet3");
	ground.push_back("pouet4");
	ground.push_back("dodo");
	ground.push_back("sleep");
	ground.push_back("rofl");

	//CONSTRUCTORS

	{
		fileName = currentPath + "constructor.log";
		outfile.open(fileName.c_str());

		S se;

		displayData(se, true, "empty basket", outfile);

		S sg(&ground);

		displayData(sg, true, "empty basket with shared ground", outfile);

		try
		{
			S except(15, "jj");
		}
		catch(const ft::exception& e)
		{
			outfile << e.what() << '\n';
		}
		
		try
		{
			S except(ground.begin(), ground.end(), 4, &ground);
		}
		catch(const ft::exception &e)
		{
			outfile << e.what() << '\n';
		}

		S sf(14, "fromage qui pue", 16, &ground);

		displayData(sf, true, "filled constructed + capacity and ground set", outfile);

		S sr(sf.begin() + 4, sf.end(), 15);

		displayData(sr, true, "range constructor", outfile);

		se.ground()->push_back("poule");

		S sf2(4, "renard", se.ground());

		displayData(sf2, true, "fill constructor with shared ground", outfile);

		outfile.close();
	}

	//INSERT

	{
		fileName = currentPath + "insert.log";
		outfile.open(fileName.c_str());
		if (outfile.fail())
			std::cerr << strerror(errno) << '\n';

		S se(25);

		S se2(12, se.ground());

		se.insert(se.end(), 2, "boule de bowling");
		displayData(se, true, "insert 2 on empty basket", outfile);

		se.insert(se.begin() + 1, "cuillere de Arthur");
		displayData(se, true, "insert 1 element somewhere in the basket", outfile);

		se.insert(se.begin() + 2, 26, "cornichons");
		displayData(se, true, "insert too much in the basket", outfile);

		se.insert(se.begin(), "et un tire bouchon");
		displayData(se, true, "1 more", outfile);

		se.enlarge_your_basket(30);

		se.insert(se.end(), ground.begin(), ground.end());
		displayData(se, true, "enlarge then range insert", outfile);

		se2.insert_force(se2.begin(), 3, "souris");
		displayData(se2, true, "insert force on empty basket", outfile);

		se2.insert_force(se2.begin() + 2, 5, "grenouille");
		displayData(se2, true, "insert force 5 more elements", outfile);

		se2.insert_force(se2.begin(), 9, "chat");
		displayData(se2, true, "insert force too much", outfile);
		
		se2.insert_force(se2.end() - 1, 3, "chien");
		displayData(se2, true, "insert force 3 elements on full basket", outfile);

		se2.insert_force(se2.end() - 4, se.begin(), se.end());
		displayData(se2, true, "range insert force on full basket", outfile);

		outfile.close();
	}

	//PUSH_BACK

	{
		fileName = currentPath + "push_back.log";
		outfile.open(fileName.c_str());

		I ie(4);

		for (int i = 3; i < 100; i += 10)
		{
			ie.push_back(i);
		}

		displayData(ie, true, "push_back too much", outfile);

		for (int i = 0; i < 3; ++i)
		{
			ie.push_back_force(i);
		}

		displayData(ie, true, "push_back_force full basket", outfile);

		ie.enlarge_your_basket(10);

		for (int i = 0; i < 15; ++i)
		{
			ie.push_back_force(i);
		}

		displayData(ie, true, "push_back_force too much on basket", outfile);

		outfile.close();
	}

	//CLEAR

	{
		fileName = currentPath + "clear.log";
		outfile.open(fileName.c_str());

		ft::vector<int> intGround(10, 10);
		I i1(12, 7, 15, &intGround);

		displayData(i1, true, "before clear", outfile);
		i1.clear();
		displayData(i1, true, "after clear", outfile);

		outfile.close();
	}

	//ASSIGN

	{
		fileName = currentPath + "assign.log";
		outfile.open(fileName.c_str());

		S se(&ground);

		se.assign(10, "chaussure");
		displayData(se, true, "assign too much on empty basket", outfile);

		se.assign(4, "bouteille");
		displayData(se, true, "assign on filled basket", outfile);

		outfile.close();
	}

	//ERASE

	{
		fileName = currentPath + "erase.log";
		outfile.open(fileName.c_str());

		S s1(8, "tomate", 20, &ground);

		s1.insert(s1.end(), 20, "patate");

		s1.erase(s1.begin());
		displayData(s1, true, "erase begin", outfile);

		s1.erase(s1.begin() + 4);
		displayData(s1, true, "erase begin + 4", outfile);

		s1.erase(s1.begin() + 2, s1.end() - 8);
		displayData(s1, true, "range erase", outfile);

		outfile.close();
	}

	//DROP

	{
		fileName = currentPath + "drop.log";
		outfile.open(fileName.c_str());

		S s1(20, &ground);

		s1.push_back("cassette");
		s1.push_back("raclette");
		s1.push_back("tartiflette");
		s1.push_back("ordinateuse");
		s1.push_back("paquerette");
		s1.push_back("trottoir");
		s1.push_back("drogue");
		s1.push_back("poussette");
		s1.push_back("tuture");
		s1.push_back("volant");
		s1.push_back("cheveu");
		s1.push_back("poil");
		s1.push_back("torse");

		displayData(s1, true, "init", outfile);

		s1.drop_back();
		displayData(s1, true, "drop_back", outfile);

		s1.drop_back();
		displayData(s1, true, "drop_back", outfile);

		s1.drop(s1.begin());
		displayData(s1, true, "drop begin", outfile);

		s1.drop(s1.begin() + 5);
		displayData(s1, true, "drop begin + 5", outfile);

		s1.drop(s1.begin() + 2, s1.end() - 2);
		displayData(s1, true, "range drop", outfile);

		s1.drop();
		displayData(s1, true, "drop all", outfile);

		outfile.close();
	}

	//PICK_UP

	{
		fileName = currentPath + "pick_up.log";
		outfile.open(fileName.c_str());

		S s1(15, &ground);

		s1.pick_up(s1.begin(), s1.ground()->begin(), s1.ground()->begin());
		displayData(s1, true, "range pick_up nothing", outfile);

		s1.pick_up(s1.begin(), s1.ground()->begin(), s1.ground()->begin() + 30);
		displayData(s1, true, "range pick_up too much", outfile);

		s1.drop();

		s1.pick_up(s1.begin(), s1.ground()->begin() + 8, s1.ground()->begin() + 11);
		displayData(s1, true, "drop then range pick_up some", outfile);

		s1.pick_up(s1.begin() + 1, s1.ground()->begin() + 8, s1.ground()->begin() + 11);
		displayData(s1, true, "range pick_up some", outfile);

		s1.pick_up(s1.begin() + 2, s1.ground()->begin() + 3);
		displayData(s1, true, "pick_up one", outfile);

		s1.fill(s1.begin(), s1.ground()->end() - 3);
		displayData(s1, true, "fill at begin from end() - 3 to end()", outfile);

		s1.fill();
		displayData(s1, true, "fill default use", outfile);

		s1.fill();
		displayData(s1, true, "fill default again (crash test)", outfile);

		outfile.close();
	}

	//POUR_OUT

	{
		fileName = currentPath + "pour_out.log";
		outfile.open(fileName.c_str());

		S s1(7, &ground);
		S s2(10, &ground);

		s2.fill();
		outfile << "quantity poured in s1 : " << s2.pour_out(s1) << '\n';
		displayData(s1, true, "get poured out too much", outfile);
		displayData(s2, true, "after pouring", outfile);

		outfile << "quantity poured in s2 : " << s1.pour_out(s2) << '\n';
		displayData(s2, true, "get poured out", outfile);
		displayData(s1, true, "after pouring", outfile);

		s1.fill();
		outfile << "quantity poured in s2 : " << s1.pour_out(s2) << '\n';
		displayData(s2, true, "get poured out", outfile);
		displayData(s1, true, "after pouring", outfile);

		s2.drop();
		outfile.close();
	}

	//GROUND_MANAGEMENT

	{
		fileName = currentPath + "ground_management.log";
		outfile.open(fileName.c_str());

		S s1(&ground);
		S sf(4, "fruit");

		outfile << "does s1 have the management of his ground ? " << s1.manage_ground() << '\n';
		outfile << "does sf have the management of his ground ? " << sf.manage_ground() << '\n';

		s1.ground(sf.ground());

		outfile << "\nground of sf : " << sf.ground() << "\n\n";
		displayData(s1, false, "after s1 took the ground of sf", outfile);

		sf.manage_ground(false);
		s1.manage_ground(true);

		outfile << "changing the ground management of sf and s1 (so now s1 will free the ground and destroy)\n";
		outfile << "does s1 have the management of his ground ? " << s1.manage_ground() << '\n';
		outfile << "does sf have the management of his ground ? " << sf.manage_ground() << '\n';
		
		sf.ground(&ground);

		displayData(sf, false, "after sf took an another ground", outfile);

		s1.swap_ground(sf);
		displayData(s1, false, "s1 after swapping ground", outfile);

		s1.swap_ground(sf);
		displayData(s1, false, "s1 after swapping ground again", outfile);

		sf.ground(0);

		displayData(sf, false, "after sf took a null ground", outfile);


		outfile.close();
	}

	//SWAP

	{
		fileName = currentPath + "swap.log";
		outfile.open(fileName.c_str());

		S s1(4, "pommade", 50, &ground);
		S s2(1, "vide");

		displayData(s2, true, "before swap", outfile);
		s2.swap(s1);
		displayData(s2, true, "after swap", outfile);
		s2.swap(s1);
		displayData(s2, true, "after swap again", outfile);
		s2.swap(s1);
		displayData(s2, true, "after swap again", outfile);

		s2.swap_basket(s1);
		displayData(s2, true, "after swap basket", outfile);
		s2.swap_basket(s1);
		displayData(s2, true, "after swap basket again", outfile);

		outfile.close();
	}

	//RELATIONAL

	{
		fileName = currentPath + "relational.log";
		outfile.open(fileName.c_str());

		I i1(4, 1, 5);
		const I i2(4, 1, 6);
		I i3(5, 1, 5);

		outfile << "must be equal : " << (i1 == i2) << '\n';
		outfile << "must not be different : " << (i1 != i2) << '\n';
		outfile << "must >= : " << (i1 >= i2) << '\n';
		outfile << "must <= : " << (i1 <= i2) << '\n';
		outfile << "must not be > : " << (i1 > i2) << '\n';
		outfile << "must not be < : " << (i1 < i2) << '\n';
		outfile << '\n';

		outfile << "must not be equal : " << (i3 == i2) << '\n';
		outfile << "must be different : " << (i3 != i2) << '\n';
		outfile << "must be >= : " << (i3 >= i2) << '\n';
		outfile << "must not be <= : " << (i3 <= i2) << '\n';
		outfile << "must be > : " << (i3 > i2) << '\n';
		outfile << "must not be < : " << (i3 < i2) << '\n';
		outfile << '\n';

		outfile << "must not be equal : " << (i2 == i3) << '\n';
		outfile << "must be different : " << (i2 != i3) << '\n';
		outfile << "must not be >= : " << (i2 >= i3) << '\n';
		outfile << "must be <= : " << (i2 <= i3) << '\n';
		outfile << "must not be > : " << (i2 > i3) << '\n';
		outfile << "must be < : " << (i2 < i3) << '\n';
		outfile << '\n';

		outfile.close();
	}

	//GET_ALLOCATOR

	{
		fileName = currentPath + "get_allocator.log";
		outfile.open(fileName.c_str());

		int a = 2;

		I i1;

		outfile << "must works, addr of an int is " << i1.get_allocator().address(a) << '\n';

		outfile.close();
	}

	//PROBLEM RESOLUTION

	{
		fileName = currentPath + "problem_solved.log";
		outfile.open(fileName.c_str());

		typedef ft::flat_basket<std::string>	Panier;
		typedef Panier::ground_type				Sol;
		typedef Panier::size_type				size_type;
		typedef ft::map<std::string, int>		DataMap;

		Panier::iterator sortIterator;
		Sol foret;
		Sol parking;
		size_type poignee;
		size_type nbInserted;
		DataMap beforeSim;
		DataMap inventory;
		DataMap afterSim;	

		Panier coffre(200, &parking);
		Panier petit(7, &foret);
		Panier gros(25, &foret);
		poignee = 4;

		for (int i = 0; i < 1000; ++i)
		{
			if (!(i % 2))
				foret.push_back("pierre");
			else if (!(i % 3))
				foret.push_back("champignon");
			else if (!(i % 5))
				foret.push_back("escargot");
			else if (!(i % 7))
				foret.push_back("detritus");
			else if (!(i % 11))
				foret.push_back("lapin");
			else
				foret.push_back("chataigne");
		}

		for (Sol::size_type i = 0; i < foret.size(); ++i)
			beforeSim[foret[i]] += 1;
	
		displayDataMap(beforeSim, "beforeSim", outfile);

		outfile << "Un vieux typique, apres une nuit de pluie d'automne, s'empresse des l'aube pour son moment prefere de l'annee, la cueillette aux champignons.\n";
		outfile << "Une fois gare au plus proche de son coin a champignons dont l'emplacement secret est jalousement garde, le vieux emporte avec lui ses deux fideles paniers de recoltes.\n";
		outfile << "L'un est petit et convient tres bien pour les manoeuvres de ramassage en terrain difficile.\n";
		outfile << "L'autre, plus imposant et plus lourd, sert de point relais dans lequel il peut deposer le contenu de son petit panier.\n";
		outfile << "Quand le gros panier est plein, il fait l'alle retour jusqu'a sa voiture pour vider le contenu du panier dans le coffre.\n";
		outfile << "La foret etant sombre, le vieux ne prendra que des poignees au hasard de ce qu'il y a sur le sol pour les inserer dans le petit panier.\n";
		outfile << "Ce n'est qu'a l'arrivee a sa voiture ou il pourra trier le contenu, dont il jettera detritus, pierres, escargots et lapins sur le parking.\n";
		outfile << "Le vieux ne rentrera pas chez lui avant d'avoir rempli son coffre a raz bord.\n\n";

		outfile << "Heureusement pour le vieux, le conteneur flat_basket est concu pour ce type de simulation, il pourra ainsi rentrer chez lui avant l'heure de la soupe.\n";

		/*
		* Pour cette simulation, je peux utiliser les methode :
		* pick_up pour ramasser des objets sur le sol.0
		* drop pour jeter du contenu sur le sol.
		* pour_out pour verser le contenu d'un panier dans un autre.
		* ground pour changer de lieu, soit le sol est celui de la foret, soit celui du parking.
		* full pour savoir quand un panier est plein.
		* Je ne peux pas utiliser la methode full pour les paniers dans la foret car l'obscurite fait qu'on y voit rien. Il faut trouver une autre methode !
		*/
		while (!coffre.full())
		{
			outfile << "la quantite dans le coffre est de " << coffre.size() << " il reste donc " << coffre.size_left() << " places.";
			if (coffre.empty() && parking.empty())
				outfile << " Le vieux rentre dans la foret.\n";
			else
				outfile << " Le vieux retourne dans la foret.\n";
			gros.ground(&foret);
			nbInserted = petit.capacity();
			while (nbInserted == petit.capacity())
			{
				nbInserted = poignee;
				while (nbInserted == poignee)
					nbInserted = petit.pick_up(petit.end(), foret.begin(), foret.begin() + poignee);
				outfile << "un bruissement signale qu'un ou plusieurs objets sont tombes au sol, le panier est donc plein, le vieux se dirige vers le gros panier et y deverse le contenu du petit panier\n";
				nbInserted = petit.pour_out(gros);
			}
			outfile << "le bruit d'objets tombant au sol signale au vieux qu'il faut ramener le contenu du gros panier a la voiture.\n";
			gros.ground(&parking);
			outfile << "Sur le parking, a la lumiere du jour, le vieux peut trier le contenu du panier avant de le verser dans sa voiture.\n";
			sortIterator = gros.begin();
			while (sortIterator != gros.end())
			{
				if (*sortIterator != "champignon" && *sortIterator != "chataigne")
					gros.drop(sortIterator);
				else
					++sortIterator;
			}
			gros.pour_out(coffre);
		}

		outfile << "le coffre est plein, le vieux rentre chez lui.\n";
		for (Panier::const_iterator cit = coffre.begin(); cit != coffre.end(); ++cit)
		{
			inventory[*cit] += 1;
			afterSim[*cit] += 1;
		}
		displayDataMap(inventory, "contenu de la voiture :", outfile);

		inventory.clear();
		for (Sol::const_iterator cit = parking.begin(); cit != parking.end(); ++cit)
		{
			inventory[*cit] += 1;
			afterSim[*cit] += 1;
		}
		displayDataMap(inventory, "Objets (ou animaux) jetes sur le parking : ", outfile);

		inventory.clear();
		for (Sol::const_iterator cit = foret.begin(); cit != foret.end(); ++cit)
		{
			inventory[*cit] += 1;
			afterSim[*cit] += 1;
		}
		displayDataMap(inventory, "Objets (ou animaux) restants dans la foret : ", outfile);

		if (beforeSim == afterSim)
			outfile << "Aucun objet n'a ete perdu lors de la simulation\n";
		else
			outfile << "des objets ont ete perdus lors de la simulation\n";
		outfile << "Fin de simulation\n";

		outfile.close();
	}
}