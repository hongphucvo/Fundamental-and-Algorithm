#ifndef _definition_h_
#include "defs.h"
#define _definition_h_
#endif


class code_Treasure {
public:
	bool found = false;
	int hash = 0;
};
struct Treasure {
	bool Paladin_Shield = false;
	bool Lancelot_Lance = false;
	bool Guivenere_Hair = false;
};

struct weapon {
	bool Excalibur = false;
	bool LionHeart = false;
};

enum NameofKnight {
	normal,
	Arthur,
	DragonKnight,
	Lancelot,
	Guinevere,
	Paladin
};
NameofKnight Char_use;

struct skill {
	int countPoison = 0, countOdin = 0, countOmega_Weapon = 0, countNina_de_Rings = 0, countLionHeart = 0;
	bool Mythril = false, Scarlet_Hakama = false;
};
struct status {
	int maxHP;
	int levelO;
	int nWin = 0;
	int nLose = 0;
	int numEvent;
	int temp_level;
	float baseDamage[5] = { 1, 1.5, 4.5, 6.5, 8.5 };
};

NameofKnight Character(knight& theKnight, skill& extra, code_Treasure& Shield, code_Treasure& Lance, code_Treasure& Hair) {
	if (theKnight.HP == 999)
	{
		return Arthur;
	}
	else if (theKnight.HP == 888) {
		Lance.found = true;
		return Lancelot;
	}
	else if (theKnight.HP == 777) {
		Hair.found = true;
		return Guinevere;
	}
	else
	{
		if (theKnight.HP == 2) {
			Shield.found = true;
			return Paladin;
		}
		else if (theKnight.HP < 2)return normal;
		else
		{
			if (theKnight.HP % 2 == 0) {
				bool value = false;
				for (int b = 1; b < theKnight.HP / 2; b++) {
					int a = theKnight.HP + (theKnight.HP * theKnight.HP) / (2 * (b - theKnight.HP));
					if (a > 0) {
						int z = theKnight.HP - (theKnight.HP * theKnight.HP) / (2 * (theKnight.HP - a));
						if (b == z) {
							int c = theKnight.HP - a - b;
							if (c * c == a * a + b * b) {
								value = true;
								break;
							}
						}
					}
				}
				if (value) {
					return DragonKnight;
				}
				else return normal;
			}
			else {
				bool value = false;
				for (int i = 3; i < theKnight.HP + 1; i++) {
					if (i == theKnight.HP)
					{
						value = true;
					}
					else if (theKnight.HP % i == 0) {
						break;
					}
				}
				if (value)
				{
					Shield.found = true;
					return Paladin;
				}
				else return normal;
			}
		}
	}
}
enum Event {
	MADBEAR = 1,
	AMAZON_MOON_BRINGER,
	ELF,
	SAXON,
	TROLL,
	TORNBERRY,
	QUEEN_OF_CARD,
	NINA_DE_RINGS,
	DURIAN,
	ANTIDOTE,
	ODIN,
	MERLIN,
	OMEGA_WEAPON,
	HADES,
	SCARLET_HAKAMA,
	LOCKED_DOOR,
	PALADIN_SHIELD = 95,
	LANCELOT_LANCE,
	GUINEVERE_HAIR,
	TAKE_EXCALIBUR,
	ULTIMECIA
};
float fr_num(int number) {
	float sum = 0;
	for (int i = 1; i < number + 1; i++) {
		if (number % i == 0) {
			sum += i;
		}
	}
	return sum / number;
}
bool MODE1(code_Treasure Consider, code_Treasure A) {
	if (Consider.hash > A.hash) {
		if (!A.found)
			return false;
		else return true;
	}
	else return true;
}



void Lose(NameofKnight Char_use, knight& theKnight, weapon& wp_use, skill& extra, status& Status, int nCastle) {
	switch (nCastle) {
	case AMAZON_MOON_BRINGER:
		if (Char_use == Guinevere) {
			break;
		}
	case MADBEAR:
	case ELF:
	case SAXON:
	case TROLL:
		float damage = Status.baseDamage[nCastle - 1] * Status.levelO * 10;
		theKnight.HP -= int(damage);
		if (theKnight.HP < 0) {
			callPhoenix(theKnight, Status.maxHP);
			if (extra.countPoison > 0)
				extra.countPoison = 0;
		}
		break;
	}
}
void Combat1_5(NameofKnight Char_use, knight& theKnight, weapon& wp_use, skill& extra, status& Status, int event_in_castle) {
	if ((Char_use % 2 == 1) || (extra.countOdin > 0) || (wp_use.LionHeart))
	{
		Status.nWin++;
		theKnight.gil += Status.baseDamage[event_in_castle - 1] * 100;
		if (extra.countPoison != 0) {
			Lose(Char_use, theKnight, wp_use, extra, Status, event_in_castle);
		}
	}
	else {
		if (theKnight.level < Status.levelO)
		{
			Status.nLose++;
			if (!extra.Mythril)
			{
				Lose(Char_use, theKnight, wp_use, extra, Status, event_in_castle);
			}
			if (extra.countPoison != 0) {
				Lose(Char_use, theKnight, wp_use, extra, Status, event_in_castle);
			}
		}
		else {
			Status.nWin++;
			theKnight.gil += Status.baseDamage[event_in_castle - 1] * 100;
			if (extra.countPoison != 0) {
				Lose(Char_use, theKnight, wp_use, extra, Status, event_in_castle);
			}
		}
	}
}
void TornBerry(NameofKnight Char_use, knight& theKnight, skill& extra, status& Status, weapon& wp_use) {
	if (extra.countPoison == 0)
	{
		if ((extra.countOdin > 0) || (Char_use == Lancelot) || (Char_use == Arthur) || (wp_use.LionHeart))
		{
			if (theKnight.level < 10) {
				theKnight.level++;
				Status.maxHP += 100;
				if (Status.maxHP > 999)Status.maxHP = 999;
			}
			Status.nWin++;
		}
		else
		{
			if (theKnight.level < Status.levelO)
			{
				Status.nLose++;
				if ((Char_use != Paladin) && (Char_use != DragonKnight)) {
					extra.countPoison++;
				}
			}
			else
			{
				Status.nWin++;
				if (theKnight.level < 10) {
					theKnight.level++;
					Status.maxHP += 100;
					if (Status.maxHP > 999)Status.maxHP = 999;
				}
			}
		}
	}
}
void Queen_of_Card(NameofKnight Char_use, knight& theKnight, skill& extra, status& Status, weapon wp_use) {
	if ((Char_use == Lancelot) || (Char_use == Arthur) || (extra.countOdin > 0) || (wp_use.LionHeart))
	{
		theKnight.gil *= 2;
		Status.nWin++;
	}
	else
	{
		if (theKnight.level < Status.levelO)
		{
			Status.nLose++;
			if (Char_use != Guinevere) {
				if (extra.Scarlet_Hakama == false) {
					theKnight.gil /= 2;
				}
			}
		}
		else
		{
			theKnight.gil *= 2;
			if (theKnight.gil > 999)theKnight.gil = 999;
			Status.nWin++;
		}
	}
}
void Nina_de_Rings(NameofKnight Char_use, knight& theKnight, weapon& wp_use, skill& extra, status& Status) {
	bool Freemoney = false;
	if (extra.Scarlet_Hakama) Freemoney = true;
	else if (Char_use == Guinevere)Freemoney = true;
	else if (Char_use == Paladin)Freemoney = true;

	bool friends = false;
	if (fr_num(theKnight.HP) == fr_num(theKnight.gil)) {
		friends = true;
		Freemoney = true;
	}

	extra.countNina_de_Rings++;
	if (extra.countNina_de_Rings == 1) {
		if ((Freemoney) || (theKnight.gil > 50)) {
			if (extra.countPoison > 0) {
				if (!Freemoney) {
					theKnight.gil -= 50;
					extra.countPoison = 0;
				}
				else extra.countPoison = 0;
			}
			if (theKnight.gil > 0) {
				if (Freemoney) {
					theKnight.HP = Status.maxHP;
				}
				else {
					if (theKnight.gil > Status.maxHP - theKnight.HP) {
						theKnight.gil -= Status.maxHP - theKnight.HP;
						theKnight.HP = Status.maxHP;
					}
					else {
						theKnight.HP += theKnight.gil;
						theKnight.gil = 0;
					}
				}
			}
		}
		if (friends) {
			wp_use.LionHeart = true;
			if (Char_use == Paladin) {
				extra.countLionHeart = -1;
			}
			else extra.countLionHeart++;
		}
	}
}
void Durian(knight& theKnight, skill& extra, status& Status, int& Petal) {
	if (extra.countPoison > 0) {
		theKnight.HP = Status.maxHP;
	}
	theKnight.HP = Status.maxHP;
	if (extra.Scarlet_Hakama == true) {
		Petal = 99;
	}
	else {
		Petal += 5;
		if (Petal > 99)
			Petal = 99;
	}
}
void Antidote(knight& theKnight, skill& extra) {
	theKnight.antidote++;
}
void Odin(skill& extra) {
	if (extra.countOdin == 0)
		extra.countOdin++;
}
void Merlin(knight& theKnight, skill& extra, status& Status) {
	if (extra.countPoison > 0) {
		extra.countPoison = 0;
	}
	if (theKnight.level < 10) {
		theKnight.level++;
		Status.maxHP += 100;
	}
	theKnight.HP = Status.maxHP;
}
void Omega_Weapon(NameofKnight Char_use, knight& theKnight, weapon& wp_use, skill& extra, status& Status) {
	if (extra.countOmega_Weapon == 0) {
		if (((theKnight.level == 10) && (wp_use.Excalibur)) || ((Char_use == DragonKnight) && (wp_use.LionHeart))) {
			Status.nWin++;
			theKnight.level = 10;
			Status.maxHP += 10;
			theKnight.gil = 999;
		}
		else {
			Status.nLose++;
			if (!extra.Mythril) {
				theKnight.HP = 0;
			}
		}
	}
}
void Hades(NameofKnight Char_use, knight& theKnight, weapon& wp_use, skill& extra, status& Status, code_Treasure Shield, code_Treasure Lance, code_Treasure Hair) {
	bool Eternal_Love = false;
	if (extra.countOdin > 0) {
		if (Char_use != DragonKnight) {
			extra.countOdin = -1;
		}
		else Eternal_Love = true;
	}
	if ((Lance.found) && Hair.found && (!wp_use.Excalibur))
		Eternal_Love = true;
	else if ((Char_use == Arthur) && (Hair.found))
		Eternal_Love = true;
	else if ((Char_use == Guinevere) && (Lance.found))
		Eternal_Love = true;
	else if (wp_use.LionHeart)
		Eternal_Love = true;

	if (Eternal_Love) {
		extra.Mythril = true;
		Status.nWin++;
	}
	else {
		if (theKnight.level < Status.levelO)
		{
			Status.nLose++;
			if (!extra.Mythril) {
				theKnight.HP = 0;
			}
		}
		else {
			Status.nWin++;
			extra.Mythril = true;
		}
	}
}
void Scarlet_Hakama(skill& extra) {
	extra.Scarlet_Hakama = true;
}
void Locked_Door(NameofKnight Char_use, knight& theKnight, int nEvent, int& j) {
	if ((Char_use != Lancelot) && (Char_use != DragonKnight)) {
		if ((j + 1) % 10 >= theKnight.level) {
			j = nEvent - 1;		//end Castle
		}
	}
}

void PaladinShield(code_Treasure& Shield, code_Treasure Lance, code_Treasure Hair, int mode) {
	if (mode == 1) {
		if (MODE1(Shield, Lance) && MODE1(Shield, Hair)) {
			Shield.found = true;
		}
	}
	else Shield.found = true;
}
void LancelotLance(code_Treasure Shield, code_Treasure& Lance, code_Treasure Hair, int mode) {
	if (mode == 1) {
		if (MODE1(Lance, Shield) && MODE1(Lance, Hair)) {
			Lance.found = true;
		}
	}
	else Lance.found = true;
}
void GuivenereHair(code_Treasure Shield, code_Treasure Lance, code_Treasure& Hair, int mode) {
	if (mode == 1) {
		if (MODE1(Hair, Shield) && MODE1(Hair, Lance)) {
			Hair.found = true;
		}
	}
	else Hair.found = true;
}
void Take_Excalibur(NameofKnight Char_use, weapon& wp_use, code_Treasure Shield, code_Treasure Lance, code_Treasure Hair) {
	if (Char_use == Arthur) wp_use.Excalibur = true;
	else
		if ((Hair.found) && (Shield.found) && (Lance.found)) wp_use.Excalibur = true;
}

bool Ultimecia(NameofKnight Char_use, knight& theKnight, weapon& wp_use, skill& extra, status& Status, int& i, int& j, int nCastle, int nEvent, int& Petal) {
	if ((wp_use.Excalibur) || (wp_use.LionHeart)) {
		Status.nWin++;
		if (extra.countPoison > 0) {
			if (Char_use != Guinevere) {
				if (!extra.Mythril) {
					if (theKnight.HP < 3)theKnight.HP = 1;
					else theKnight.HP /= 3;
				}
			}
		}
		return true;
	}
	else {
		Status.nLose++;
		if (Char_use != Guinevere) {
			if (!extra.Mythril) {
				if (theKnight.HP < 3)theKnight.HP = 1;
				else theKnight.HP /= 3;
			}
		}
		return false;
	}
}
void End_game(int& i, int& j, int nEvent, int nCastle) {
	i = nCastle;
	j = nEvent;
}
void Check(knight& theKnight, weapon& wp_use, skill& extra, status& Status, int& i, int& j, int nEvent, int nCastle, int& Petal, int* bFlag, castle arrCastle, code_Treasure Shield, code_Treasure Lance, code_Treasure Hair, bool fightUltimecia) {
	if (fightUltimecia) {
		i = nCastle;
		j = nEvent;
	}

	if (theKnight.HP < 1) {
		callPhoenix(theKnight, Status.maxHP);
		if (extra.countPoison > 0)
			extra.countPoison = 0;
	}
	else if (theKnight.HP > Status.maxHP)theKnight.HP = Status.maxHP;

	if (theKnight.gil > 999)theKnight.gil = 999;

	if (nEvent == 0) {
		theKnight.level++;
		Status.maxHP += 100;
	}
	else if (j == nEvent - 1) {
		theKnight.level++;
		Status.maxHP += 100;
		if (i == nCastle - 1) {
			i = -1;
			j = -1;
		}
	}

	if (Status.maxHP > 999)Status.maxHP = 999;

	if (theKnight.level > 10)
		theKnight.level = 10;

	if (extra.countPoison > 0) {
		extra.countPoison++;
		if (theKnight.HP < 1) {
			callPhoenix(theKnight, Status.maxHP);
			extra.countPoison = 7;
		}
		if (extra.countPoison > 6) {
			extra.countPoison = 0;
		}
		else if (theKnight.antidote > 0) {
			theKnight.antidote--;
			extra.countPoison = 0;
		}
	}

	if (extra.countNina_de_Rings > 0) {
		extra.countNina_de_Rings++;
		if (extra.countNina_de_Rings > 6) {
			extra.countNina_de_Rings = 0;
		}
	}
	if (extra.countLionHeart > 0) {
		extra.countLionHeart++;
		if (extra.countLionHeart > 6) {
			extra.countLionHeart = 0;
			wp_use.LionHeart = false;

		}
	}

	if (extra.countOdin > 0) {
		extra.countOdin++;
		if (extra.countOdin > 6) {
			extra.countOdin = 0;
		}
	}

	if (Petal != 0) {
		Petal--;
		if (Petal == 0) {
			if (Char_use != Arthur) {
				bFlag = 0;
				End_game(i, j, nEvent, nCastle);
			}
		}
	}
}



report* walkthrough(knight& theKnight, castle arrCastle[], int nCastle, int mode, int nPetal)
{
	report* pReturn;
	int bFlag = 1;
	//fighting for the existence of mankind here

	skill extra;
	status Status;
	weapon wp_use;
	Status.maxHP = theKnight.HP;
	code_Treasure Shield;
	code_Treasure Lance;
	code_Treasure Hair;

	Char_use = Character(theKnight, extra, Shield, Lance, Hair);
	if (mode == 1) {
		Shield.hash = hash(95);
		Lance.hash = hash(96);
		Hair.hash = hash(97);
	}
	else if (mode == 2) {
		knight tempKnight = theKnight;
		knight resultKnight = theKnight;
		report* Max = walkthrough(tempKnight, arrCastle, nCastle, 3, nPetal);
		int numofevent = 0, tempPetal = nPetal;

		for (int count = 0; count < nCastle; count++) {
			numofevent += arrCastle[count].nEvent;
		}
		if (tempPetal - Max->nPetal >= numofevent) {
			Max->nPetal = -100;
		}

		for (int i = nCastle - 1; i > -1; i--) {
			for (int k = i; k <= nCastle; k++) {
				castle Arrtemp;
				Arrtemp = arrCastle[i];
				arrCastle[i] = arrCastle[k];
				arrCastle[k] = Arrtemp;
				tempKnight = theKnight;
				report* RUN = walkthrough(tempKnight, arrCastle, nCastle, 0, nPetal);

				if (tempPetal - RUN->nPetal >= numofevent) {
					if (Max->nPetal < RUN->nPetal) {
						Max = RUN;
						resultKnight = tempKnight;
					}
					else delete RUN;
				}
			}
		}
		theKnight = resultKnight;
		return Max;
	}
	for (int i = 0; i < nCastle; i++) {
		if (arrCastle[i].nEvent == 0) {
			if (theKnight.level < 10) {
				theKnight.level++;
				Status.maxHP += 100;
				if (Status.maxHP > 999)Status.maxHP = 999;
			}
		}

		for (int j = 0; j < arrCastle[i].nEvent; j++) {
			bool fightUltimecia = false;
			int b = (j + 1) % 10;
			Status.levelO = (j + 1) > 6 ? (b > 5 ? b : 5) : b;
			switch (arrCastle[i].arrEvent[j])
			{
			case MADBEAR:
			case AMAZON_MOON_BRINGER:
			case ELF:
			case SAXON:
			case TROLL:
				Combat1_5(Char_use, theKnight, wp_use, extra, Status, arrCastle[i].arrEvent[j]);
				break;
			case TORNBERRY:
				TornBerry(Char_use, theKnight, extra, Status, wp_use);
				break;
			case QUEEN_OF_CARD:
				Queen_of_Card(Char_use, theKnight, extra, Status, wp_use);
				break;
			case NINA_DE_RINGS:
				Nina_de_Rings(Char_use, theKnight, wp_use, extra, Status);
				break;
			case DURIAN:
				Durian(theKnight, extra, Status, nPetal);
				break;
			case ANTIDOTE:
				Antidote(theKnight, extra);
				break;
			case ODIN:
				Odin(extra);
				break;
			case MERLIN:
				Merlin(theKnight, extra, Status);
				break;
			case OMEGA_WEAPON:
				Omega_Weapon(Char_use, theKnight, wp_use, extra, Status);
				break;
			case HADES:
				Hades(Char_use, theKnight, wp_use, extra, Status, Shield, Lance, Hair);
				break;
			case SCARLET_HAKAMA:
				Scarlet_Hakama(extra);
				break;
			case LOCKED_DOOR:
				Locked_Door(Char_use, theKnight, arrCastle[i].nEvent, j);
				break;
			case PALADIN_SHIELD:
				PaladinShield(Shield, Lance, Hair, mode);
				break;
			case LANCELOT_LANCE:
				LancelotLance(Shield, Lance, Hair, mode);
				break;
			case GUINEVERE_HAIR:
				GuivenereHair(Shield, Lance, Hair, mode);
				break;
			case TAKE_EXCALIBUR:
				Take_Excalibur(Char_use, wp_use, Shield, Lance, Hair);
				break;
			case ULTIMECIA:
				fightUltimecia = Ultimecia(Char_use, theKnight, wp_use, extra, Status, i, j, nCastle, arrCastle[i].nEvent, nPetal);
				break;
			}
			Check(theKnight, wp_use, extra, Status, i, j, arrCastle[i].nEvent, nCastle, nPetal, &bFlag, arrCastle[i], Shield, Lance, Hair, fightUltimecia);
		}
	}
	// success or failure?
	if (bFlag != 0)bFlag = 1;
	pReturn = (bFlag) ? new report : NULL;
	if (pReturn != NULL) {
		pReturn->nLose = Status.nLose;
		pReturn->nPetal = nPetal;
		pReturn->nWin = Status.nWin;
	}
	return pReturn;
}