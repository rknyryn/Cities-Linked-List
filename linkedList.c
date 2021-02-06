#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>

//Dügümler icin kullanilacak yapilar
struct City
{
	int plateCode;
	char cityName[15];
	char region[2];
	int neighborCount;
	struct City *next;
	struct City *previous;
	struct NeighborCity *neighbor;
};

struct NeighborCity
{
	int plateCode;
	char cityName[15];
	struct NeighborCity *next;
};

//Kolay erisim icin tur donusumu
typedef struct City city;
typedef struct NeighborCity neighborCity;

void cursor(int cursor, int option)
{
	if(cursor == option)
		printf("-- -> ");
	else
		printf("      ");
}

//Eklenecek sehir icin dugum olusturulmasi
city *createCity(city *node, int plateCode, char *cityName, char *region)
{
	//Bellekten yer ayiriyor
	node = (city *)malloc(sizeof(city));
	//Bilgilerin atamasi
	node->plateCode = plateCode;
	strcpy(node->cityName, cityName);
	strcpy(node->region, region);
	node->next = NULL;
	node->previous = NULL;
	node->neighbor = NULL;
	node->neighborCount = 0;
	return node;
}

//Eklencek komsu sehir icin dugum olusturulmasý
neighborCity *createNeighbor(neighborCity *node, int plateCode, char *cityName)
{
	//Bellekten yer ayiriyor
	node = (neighborCity *)malloc(sizeof(neighborCity));
	//Bilgilerin atamasý
	node->plateCode = plateCode;
	strcpy(node->cityName, cityName);
	node->next = NULL;
	return node;
}

//Ayi plaka kodunda sehir eklenmesinin kontrolunu yapmak icin
bool cityControl(city *node, int plateCode)
{
	//Dugumlerde gezmek icin isaretci
	city *iter = node;
	//Bos dugum olana kadar dongu
	while(iter != NULL)
	{
		if(iter->plateCode == plateCode) //Eger ayni plaka kodunda sehir var ise false donduruyor
			return false;
		iter = iter->next;
	}
	return true; //Yok ise true donduruyor
}
//Listeye sehir ekleme
city *addCity(city *node, int plateCode, char *cityName, char *region)
{
	//listede gezinmek icin bir isaretci
	city *iter = node;
	//Hic sehir eklenmemis ise
	if(iter == NULL)
	{
		iter = createCity(iter, plateCode, cityName, region);
		return iter;
	}
	if(cityControl(iter, plateCode))
	{
		//Baþa ekleme
		if(iter->plateCode > plateCode)
		{
			city *newCity = createCity(iter->next, plateCode, cityName, region);
			newCity->next = iter;
			iter->previous = newCity;
			return newCity;
		}
		//Eklenecek yeri bulmak için plaka kodlarý kontrol ediliyor
		//Plaka kodu kucuk oldugu surece ve ileride dugum oldugu surece ilerliyor
		while(iter->plateCode < plateCode && iter->next != NULL)
			iter = iter->next;
		//Eklenecek sehir icin yeni bir dugum olusturuluyor
		city *newCity = createCity(iter->next, plateCode, cityName, region);
		//Eger listede 1 adet dugum varsa ve eklenecek sehrin plaka kodu daha buyuk ise
		if(iter->plateCode < plateCode && iter->next == NULL)
		{
			iter->next = newCity;
			newCity->previous = iter;
			return node;
		}
		//Listede çok sayýda dugum varsa; araya ekleme yapýyor
		newCity->next = iter;
		newCity->previous = iter->previous;
		iter->previous = newCity;
		newCity->previous->next = newCity;
		return node;
	}
}

//Listeden sehir silme
city *deleteCity(city *node)
{
	int plateCode;
	printf("Silmek istediginiz sehrin plaka kodunu giriniz: ");
	scanf("%d", &plateCode);
	
	//listede gezinmek icin bir isaretci
	city *iter = node;
	//Silinecek sehir ilk dugum ise
	if(iter->plateCode == plateCode)
	{
		city *temp = iter->next;
		//Dugumu bellekten serbest birakma
		free(iter);
		//Yeni baslangic dugumunu geri donduruyor
		return temp;
	}
	//Silinecek sehrin plaka koduna esit degil ve bir sonraki dugum NULL degil ise sonraki dugume geciyor
	//Isaretci silinecek dugumden onceki dugumde duruyor
	while(iter->next != NULL && iter->next->plateCode != plateCode)
		iter = iter->next;
	//Eðer boyle bir plaka kodu listede yok ise
	if(iter->plateCode != plateCode && iter->next == NULL)
	{
		return node;	
	}
	//Silincek sehri gecici bir degiskene aliniyor
	city *temp = iter->next;
	//Onceki dugum, silinecek dugumden sonrakine baglaniyor
	if(temp->next == NULL)
	{
		iter->next = NULL;
		//Baslangic dugumu degismedigi icin ayni dugum geri donduruluyor
		return node;
	}
	iter->next = temp->next;
	//Silinecek dugumden sonraki dugumun öncesi isaretciye baglaniyor
	temp->next->previous = iter;
	//Gecici degiskene alinan dugumun bellekteki yeri serbest birakiliyor
	free(temp);
	//Baslangic dugumu degismedigi icin ayni dugum geri donduruluyor
	return node;
}

//Listedeki sehire komsu sehir ekleme
void addNeighborCity(city *node, int cityPlateCode, int neighborPlateCode, char *neighborName)
{
	//listede gezinmek icin bir isaretci
	city *iter = node;
	//Komsu eklenecek sehirin listede oldugunu kontrol etmek icin dugumler geziliyor
	while(iter->plateCode != cityPlateCode && iter->next != NULL)	
		iter = iter->next;
	//Eger listede boyle bir sehir yok ise return edilerek cikiliyor
	if(iter->plateCode != cityPlateCode && iter->next == NULL)
		return;
	//Sehrin hic komsusu yok ise
	if(iter->neighbor == NULL)
	{
		iter->neighbor = createNeighbor(iter->neighbor, neighborPlateCode, neighborName);
		iter->neighborCount += 1;
		return;
	}
	//Eklenecek komsu olusturuluyor
	neighborCity *newNeighbor = createNeighbor(iter->neighbor->next, neighborPlateCode, neighborName);
	//Basa ekleme
	if(iter->neighbor->plateCode > neighborPlateCode)
	{
		newNeighbor->next = iter->neighbor;
		iter->neighbor = newNeighbor;
		iter->neighborCount += 1;
		return;
	}
	//Sehrin komsularinda gezinerek, eklenecek yer bulunuyor
	while(iter->neighbor->next != NULL && iter->neighbor->next->plateCode < neighborPlateCode)
		iter->neighbor = iter->neighbor->next;
	
	//Araya ekleme
	if(iter->neighbor->next != NULL)	 
		newNeighbor->next = iter->neighbor->next;
	iter->neighbor->next = newNeighbor;
	iter->neighborCount += 1;
}

//Listeden komsu silme
void deleteNeighbor(city *node)
{
	int cityPlateCode;
	int neighborPlateCode;
	printf("Hangi sehirden komsuluk silmek istiyorsunuz(plaka kodu): ");
	scanf("%d", &cityPlateCode);
	printf("Silmek istediginiz komsulugun plaka kodu: ");
	scanf("%d", &neighborPlateCode);
	
	//listede gezinmek icin bir isaretci
	city *iter = node;
	neighborCity *temp;
	//Sehirin listede oldugunu kontrol etmek icin dugumler geziliyor
	while(iter->plateCode != cityPlateCode && iter->next != NULL)	
		iter = iter->next;
	//Eger listede boyle bir sehir yok ise return edilerek cikiliyor
	if(iter->plateCode != cityPlateCode && iter->next == NULL)
		return;
	//Silinecek komsu sehir ilk dugum ise
	if(iter->neighbor->plateCode == neighborPlateCode)
	{
		temp = iter->neighbor;
		if(temp->next == NULL)
			iter->neighbor = NULL;
		else
			iter->neighbor = temp->next;
		free(temp);
		iter->neighborCount --;
		return;
	}
	//Silinecek komsu sehir dugumlerde araniyor
	while(iter->neighbor!= NULL && iter->neighbor->next->plateCode != neighborPlateCode)
		iter->neighbor = iter->neighbor->next;
	//Eger boyle bir komsuluk yoksa return edilerek cikiliyor
	if(iter->neighbor == NULL)
		return;
	//Isaretci silinecek dugumden onceki dugumde duruyor
	//Silinecek dugum gecici degiskene aliniyor
	//Isaretci gecici dugumden sonraki dugumu gosteriyor
	//Gecici degisken bellekten serbest birakiliyor
	temp = iter->neighbor->next;
	iter->neighbor->next = temp->next;
	free(temp);
	iter->neighborCount --;
}

//Sehirlerin genel bilgilerini yazdiriyor
void writeCities(city *node)
{
	//Þehirleri gezmek için bir isaretci
	city *iter = node;
	//Komþularý gezmek için bir isaretci
	neighborCity *iterN;
	//Isaretci bos degeri gostermedigi surece dugumlerde ilerliyor
	while(iter != NULL)
	{
		//Sehrin bilgilerini yaziyor
		printf("\n%d-%s (%s)", iter->plateCode, iter->cityName , iter->region);
		iterN = iter->neighbor;
		while(iterN != NULL)
		{
			//Sehrin komsu sehirlerinin adlarini yaziyor
			printf(" -> %d-%s",iterN->plateCode, iterN->cityName);
			iterN = iterN->next;
		}
		iter = iter->next;
	}
}

//Plaka koduna gore sehir arama
void searchCitiesFromPlateCode(city *node)
{
	int plateCode;
	printf("Aradiginiz sehrin plaka kodunu giriniz: ");
	scanf("%d", &plateCode);
	
	//Dugumleri gezmek icin isaretci
	city *iter = node;
	//Komþularý gezmek için bir isaretci
	neighborCity *iterN;
	while(iter != NULL && iter->plateCode != plateCode)
		iter = iter->next;
	//Eger sehir varsa sehrin bilgilerini ekrana yazar
	if(iter != NULL)
	{
		iterN = iter->neighbor;
		printf("\n%d-%s\nBolge: %s\nKomsu Sayisi: %d", iter->plateCode, iter->cityName, iter->region, iter->neighborCount);
		printf("\nKomsulari: ");
		while(iterN != NULL)
		{
			printf("\n	%d-%s", iterN->plateCode, iterN->cityName);
			iterN = iterN->next;
		}
	}
}

void addCityManuel(city *node)
{
	int plateCode;
	char cityName[15];
	char region[2];
	printf("Plaka kodunu giriniz: ");
	scanf("%d", &plateCode);
	printf("Sehir adini giriniz: ");
	scanf("%s", cityName);
	printf("Bolgesini giriniz: ");
	scanf("%s", region);
	
	node = addCity(node, plateCode, cityName, region);
}

void addNeighborManuel(city *node)
{
	int cityPlateCode;
	int neighborPlateCode;
	char neighborName[15];
	printf("Komsuluk eklemek istediginiz sehrin plaka kodunu giriniz: ");
	scanf("%d", &cityPlateCode);
	printf("Komsu sehrin plaka kodunu giriniz: ");
	scanf("%d", &neighborPlateCode);
	printf("Komsu sehrin adini giriniz: ");
	scanf("%s", neighborName);
	
	addNeighborCity(node, cityPlateCode, neighborPlateCode, neighborName);
}

void menu(city *node)
{
	int key;
	int c = 0;//cursor
	while(1)
	{
		system("cls");
		cursor(c, 0); printf("Sehirleri listele\n");
		cursor(c, 1); printf("Plaka kodu ile arama\n");
		cursor(c, 2); printf("Sehir ekleme\n");
		cursor(c, 3); printf("Sehir silme\n");
		cursor(c, 4); printf("Komsu sehir ekleme\n");
		cursor(c, 5); printf("Komsu sehir silme\n");
		cursor(c, 6); printf("Cikis\n");
		key = getch();
		
		if(key == 80 && c != 6)
			c ++;
		else if(key == 80 && c == 6)
			c = 0;
		else if(key == 72 && c != 0)
			c --;
		else if(key == 72 && c == 0)
			c = 6;
		else if(key == 13)
		{
			system("cls");
			switch(c)
			{	
				case 0:
					writeCities(node);
					break;
				case 1:
					searchCitiesFromPlateCode(node);
					break;
				case 2:
					addCityManuel(node);
					break;
				case 3:
					node = deleteCity(node);
					break;
				case 4:
					addNeighborManuel(node);
					break;
				case 5:
					deleteNeighbor(node);
					break;
				case 6:
					exit(0);
					break;
			}
			printf("\n\nMenuye donmek icin herhangi bir tusa basiniz...");
			key = getch();
		}	
	}
}

//Dosyadan gonderilen satirlardaki verileri dugumlere eklemek icin
city *addCityFromFile(city *node, char *line)
{
	//Verileri almak icin olusturulmus degiskenler
	int i=0;
	int plateCode;
	char cityName[15];
	char region[2];
	//strtok fonksiyonu ile dosya icerisindeki format baz alinarak gelen veri bolunuyor
	char * token = strtok(line, ",");
   	while( token != NULL ) 
	{
		//Ilk veri sehrin plaka kodu
		if(i==0)
			plateCode = atoi(token);
		//Ikinci veri sehrin ismi
		else if(i == 1)
			strcpy(cityName, token);
		//Ucuncu veri sehrin bolgesi
		else if(i == 2)
			strcpy(region, token);
		token = strtok(NULL, ",");
	 	i ++;
    }
    return addCity(node, plateCode, cityName, region);
}

//cities.txt dosyasindan sehirleri alma
city *readCitiesFromFile(city *node)
{
	//Dosya icerisinde satir satir okuma yapiliyor
	//Her bir satirdaki sehir dugume eklenmesi icin fonksiyona yonlendiriliyor
	FILE *file = fopen("cities.txt","r");
	char *line;
	while(!feof(file))
	{
		fscanf(file,"%s", line);
		node = addCityFromFile(node, line);
	}
	fclose(file);
	return node;
}

bool cityNameControl(char *cityName1, char *cityName2)
{
	int i=0;
	while(cityName1[i] != '\0')
	{
		if(cityName1[i] != cityName2[i])
			return false;
		i++;
	}
	return true;
}

void addNeighborFromFile(city *node, char *line)
{
	int cityPlateCode;
	int neighborPlateCode;
	char neighborCityName[15];
	
	char * token = strtok(line, ",");
	cityPlateCode = atoi(token);
	token = strtok(NULL, ",");
	token = strtok(NULL, ",");
	token = strtok(NULL, ",");
	
	city *iter = node;
	while(token != NULL)
	{	
		while(cityNameControl(iter->cityName, token) == false && iter != NULL)
			iter = iter->next;

		neighborPlateCode = iter->plateCode;
		strcpy(neighborCityName,iter->cityName);
	
		addNeighborCity(node, cityPlateCode, neighborPlateCode, neighborCityName);
		token = strtok(NULL, ",");	
		iter = node;
	}
}

city *readNeighborFromFile(city *node)
{
	FILE *file = fopen("cities.txt","r");
	char *line;
	int cityPlateCode;
	while(!feof(file))
	{
		fscanf(file,"%s", line);
		addNeighborFromFile(node, line);
	}
	fclose(file);
	return node;
}

int main()
{
	city *node = NULL;
	node = readCitiesFromFile(node);
	node = readNeighborFromFile(node);
	menu(node);
	return 0;
}
