# include "../includes/flat_basket.hpp"
# include <iostream>
# include <string>
# include "../includes/map.hpp"

template < typename T >
void    displayDataMap(T& container, const char* name)
{
	typename T::iterator     it;

	it = container.begin();
	std::cout << name << " content :" << '\n';
	while (it != container.end())
	{
		std::cout << (*it).first << " => " << (*it).second << '\n';
		it++;
	}
	std::cout << '\n';
}

template < typename T >
void    displayContent(const T& container, const char* name)
{
	typename T::const_iterator it;
	typename T::const_iterator end;

	it = container.begin();
	end = container.end();
	std::cout << "Content";
	if (name)
		std::cout << " of " << name;
	std::cout << " :\n";
	while (it != end)
	{
		std::cout << *it << '\n';
		it++;
	}
	std::cout << '\n';
}

template <class T>
static void	displayData(const ft::flat_basket<T>& f, bool displayGround, const char* name)
{
	std::cout << "Data";
	if (name)
		std::cout << " of " << name;
	std::cout << " :\n";
	std::cout << "size = " << f.size();
	std::cout << "\ncapacity = " << f.capacity();
	std::cout << "\nsize left = " << f.size_left();
	std::cout << "\nmax size = " << f.max_size();
	std::cout << "\nground ptr = " << f.ground();
	std::cout << "\nmanage his own ground = " << f.manage_ground();
	std::cout << '\n';
	displayContent(f, name);
	if (displayGround)
		displayContent(*f.ground(), "ground");
}

void	flat_basket_tests()
{

	typedef ft::flat_basket<std::string>	Panier;
	typedef Panier::ground_type	Sol;
	typedef Panier::size_type		size_type;
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
	
	displayDataMap(beforeSim, "beforeSim");

	std::cout << "Un vieux typique, apres une nuit de pluie d'automne, s'empresse des l'aube pour son moment prefere de l'annee, la cueillette aux champignons.\n";
	std::cout << "Une fois gare au plus proche de son coin a champignons dont l'emplacement secret est jalousement garde, le vieux emporte avec lui ses deux fideles paniers de recoltes.\n";
	std::cout << "L'un est petit et convient tres bien pour les manoeuvres de ramassage en terrain difficile.\n";
	std::cout << "L'autre, plus imposant et plus lourd, sert de point relais dans lequel il peut deposer le contenu de son petit panier.\n";
	std::cout << "Quand le gros panier est plein, il fait l'alle retour jusqu'a sa voiture pour vider le contenu du panier dans le coffre.\n";
	std::cout << "La foret etant sombre, le vieux ne prendra que des poignees au hasard de ce qu'il y a sur le sol pour les inserer dans le petit panier.\n";
	std::cout << "Ce n'est qu'a l'arrivee a sa voiture ou il pourra trier le contenu, dont il jettera detritus, pierres, escargots et lapins sur le parking.\n";
	std::cout << "Le vieux ne rentrera pas chez lui avant d'avoir rempli son coffre a raz bord.\n\n";

	std::cout << "Heureusement pour le vieux, le conteneur flat_basket est concu pour ce type de simulation, il pourra ainsi rentrer chez lui avant l'heure de la soupe.\n";

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
		std::cout << "la quantite dans le coffre est de " << coffre.size() << " il reste donc " << coffre.size_left() << " places.";
		if (coffre.empty() && parking.empty())
			std::cout << " Le vieux rentre dans la foret.\n";
		else
			std::cout << " Le vieux retourne dans la foret.\n";
		gros.ground(&foret);
		nbInserted = petit.capacity();
		while (nbInserted == petit.capacity())
		{
			nbInserted = poignee;
			while (nbInserted == poignee)
				nbInserted = petit.pick_up(petit.end(), foret.begin(), foret.begin() + poignee);
			std::cout << "un bruissement signale qu'un ou plusieurs objets sont tombes au sol, le panier est donc plein, le vieux se dirige vers le gros panier et y deverse le contenu du petit panier\n";
			nbInserted = petit.pour_out(gros);
		}
		std::cout << "le bruit d'objets tombant au sol signale au vieux qu'il faut ramener le contenu du gros panier a la voiture.\n";
		gros.ground(&parking);
		std::cout << "Sur le parking, a la lumiere du jour, le vieux peut trier le contenu du panier avant de le verser dans sa voiture.\n";
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

	std::cout << "le coffre est plein, le vieux rentre chez lui.\n";
	for (Panier::const_iterator cit = coffre.begin(); cit != coffre.end(); ++cit)
	{
		inventory[*cit] += 1;
		afterSim[*cit] += 1;
	}
	displayDataMap(inventory, "contenu de la voiture :");

	inventory.clear();
	for (Sol::const_iterator cit = parking.begin(); cit != parking.end(); ++cit)
	{
		inventory[*cit] += 1;
		afterSim[*cit] += 1;
	}
	displayDataMap(inventory, "Objets (ou animaux) jetes sur le parking : ");

	inventory.clear();
	for (Sol::const_iterator cit = foret.begin(); cit != foret.end(); ++cit)
	{
		inventory[*cit] += 1;
		afterSim[*cit] += 1;
	}
	displayDataMap(inventory, "Objets (ou animaux) restants dans la foret : ");

	if (beforeSim == afterSim)
		std::cout << "Aucun objet n'a ete perdu lors de la simulation\n";
	else
		std::cout << "des objets ont ete perdus lors de la simulation\n";
	std::cout << "Fin de simulation\n";
}