#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define L_STR 4



//****STRUTTURE****

typedef enum {red,black} colours;


typedef struct EL2 stati;

//R-B Tree stati raggiungibili
typedef struct EL { int          dest;
				    char         input;
			        char		 testina;
			        char		 output;
				    struct EL*   dx;
				    struct EL*   sx;
				    struct EL*   centro;
				    struct EL*   padre;
				    colours	     colore;
			      } stati_ragg;



typedef stati_ragg* RBT;

//albero di alberi
struct EL2 { int     	   id;
			 stati*  	   padre;
			 stati*        dx;
			 stati*        sx;
			 colours       colore;
			 RBT		   root;
		   };


typedef stati* alberone;




//nastro biinfinito
typedef struct EL3{ char		  input;
 				    struct EL3*   succ;
 				    struct EL3*   prec;
 			      } slot;


typedef slot* nastro;



//lista stati accettazione
typedef struct EL4{ int statoAcc;
					struct EL4*	succ;
					struct EL4*	prec;
				  }elemento;
				  
typedef elemento* lista;

typedef struct EL5{ nastro       	buPosizione;
					int		     	buStato;
					int		     	salti;
					unsigned int	numeroTransizioni;
					struct EL5*  	succ;	   
				  }istantanea;
				 
typedef istantanea* listaIstantanee;



//****FUNZIONI****

//----------------
//-ALBERONE STATI-
//----------------
//inizializza albero stati
//Variabili passate per indirizzo
void creaStati(alberone* alberoStati, int statoI,char in, char out,char testina,int statoF)
{
	RBT creaStatiRagg(int statoF,char in,char out,char testina);
	
	alberone albero1;
	albero1 = malloc(sizeof(stati));
	
	albero1->id = statoI;
	albero1->padre = NULL;
	albero1->dx = NULL;
	albero1->sx = NULL;
	albero1->colore = black;
	albero1->root = creaStatiRagg(statoF, in, out, testina);
	*alberoStati = albero1;
}		


//ALBERONE STATI
//inserisce i nuovi stati nell'alberone e chiama la procedura per creare il rb-tree degli stati ragg
//oppure chiama la procedura per aggiungere un nuovo stato ragg. specialCase serve a distinguere la chiamata a inserisciStati
// quando viene fatta dopo  aver letto acc
void inserisciStati(alberone* alberoStati, int statoI,char in, char out,char testina,int statoF,int specialCase)	
{
	alberone padre,alberoCorr;
		
	padre = NULL;
	alberoCorr = *alberoStati;
		
	while(alberoCorr != NULL  &&  alberoCorr->id != statoI)
	{
		padre = alberoCorr;
			
		if(alberoCorr->id < statoI)	alberoCorr = alberoCorr->dx;
			
		else	alberoCorr = alberoCorr->sx;
	}
		
		
	//elemento già presente in albero
	if (alberoCorr != NULL  &&  alberoCorr->id == statoI)
	{
		void inserisciStatoRagg(alberone *alberoStati1, int statoF,char in,char out,char testina);	
		
		if(1 == specialCase)	return;
		
		inserisciStatoRagg(&alberoCorr,statoF,in,out,testina);

	}
		
	//elemento non presente nell'albero
	else
	{
		void bilanciaStati(alberone* nodo);
		RBT creaStatiRagg(int statoF,char in,char out,char testina);
			
		alberoCorr = malloc(sizeof(stati));
		alberoCorr->id = statoI;
		alberoCorr->dx = NULL;
		alberoCorr->sx = NULL;
		alberoCorr->padre = padre;
		
		if(padre->id < statoI)	padre->dx = alberoCorr;
		
		else	padre->sx = alberoCorr;

		bilanciaStati(&alberoCorr);
		
		if(0 == specialCase)
		{
			alberoCorr->root = creaStatiRagg(statoF,in,out,testina);
		}
		
		else	alberoCorr->root = NULL;
		
		while( (*alberoStati)->padre != NULL)
		{
			*alberoStati = (*alberoStati)->padre;
		}
	}	

}


//ALBERONE STATI
//effetua una rotazione dx o sx nell'alberone 
void ruotaStati(char lato,alberone* root)
{
	alberone padre,figlio;
	figlio = NULL;
	padre = (*root)->padre;
	
	if(lato == 's')
	{
		figlio = (*root)->dx;
		(*root)->dx = figlio->sx;
		
		if(figlio->sx != NULL)	figlio->sx->padre = *root;
		
		figlio->sx = *root;
		(*root)->padre = figlio;
		figlio->padre = padre;
		
		if(padre != NULL)
		{
			if(padre->sx == *root)	padre->sx = figlio;
			
			else	padre->dx = figlio;
		}
	}
	
	else if(lato == 'd')
	{
		figlio = (*root)->sx;
		(*root)->sx = figlio->dx;
		
		if(figlio->dx != NULL)	figlio->dx->padre = *root;
		
		figlio->dx = *root;
		(*root)->padre = figlio;
		figlio->padre = padre;
		
		if(padre != NULL)
		{
			if(padre->dx == *root)	padre->dx = figlio;
			
			else	padre->sx = figlio;
		}
	}
	
	return;
}

//ALBERONE STATI
//bilancia alberone stati
void bilanciaStati(alberone* nodo)
{
	void ruotaStati(char lato,alberone* root);

	alberone alberone1 = *nodo;
	alberone1->colore = red;
	
	while(alberone1 != NULL)
	{
		alberone padre = alberone1->padre;
		alberone nonno = NULL;
		alberone zio = NULL;
		
		if(padre != NULL) nonno = padre->padre;
			
		if(nonno != NULL) 
		{
			if(nonno->sx == padre)	zio = nonno->dx;
			else	zio = nonno->sx;
		}
		
		//CASO1: se non ha padre il nodo è radice, coloro di nero e termino
		if(padre == NULL)
		{
			alberone1->colore = black;
			alberone1 = NULL;
		}
		
		//CASO2:padre nero
		else if(padre->colore == black)		alberone1 = NULL;
		
		//CASO3:zio rosso
		else if(zio != NULL  &&  zio->colore == red)
		{
			padre->colore = black;
			zio->colore = black;
			nonno->colore = red;
			alberone1 = nonno;
		}
		
		else
		{
			if(alberone1 == padre->dx  &&  padre == nonno->sx)	//CASO4a:
			{
				ruotaStati('s',&padre);
				alberone1 = padre;
			}
			
			else if(alberone1 == padre->sx  &&  padre == nonno->dx)	//CASO4b:
			{
				ruotaStati('d',&padre);
				alberone1 = padre;
			}
			
			else
			{
				if(alberone1 == padre->sx  &&  padre == nonno->sx)	ruotaStati('d',&nonno);	//CASO5a:
				
				else if(alberone1 == padre->dx  &&  padre == nonno->dx)	ruotaStati('s',&nonno);	//CASO5b:
			
				padre->colore = black;
				nonno->colore = red;
				alberone1 = NULL;
			}
		}
		
	}
	
	return;
}

//ALBERONE
//visita l'albero degli stati in cerca di un elemento
alberone cercaStati(alberone* alberoStati, int ricercato)
{

	alberone cursoreAlberone = *alberoStati;
	
	while(cursoreAlberone != NULL  &&  cursoreAlberone->id != ricercato)
	{
		if(cursoreAlberone->id < ricercato)	cursoreAlberone = cursoreAlberone->dx;
		
		else	cursoreAlberone = cursoreAlberone->sx;
		
	}

	return cursoreAlberone;
}


//--------------------
//-RB TREE STATI RAGG-
//--------------------
//Variabili passate per indirizzo
RBT creaStatiRagg(int statoF,char in,char out,char testina)
{
	RBT stati_ragg1;
	stati_ragg1 = malloc(sizeof(stati_ragg));
	
	stati_ragg1->dest = statoF;
	stati_ragg1->input = in;
	stati_ragg1->output = out;
	stati_ragg1->testina = testina;
	stati_ragg1->dx = NULL;
	stati_ragg1->sx = NULL;
	stati_ragg1->centro = NULL;
	stati_ragg1->padre = NULL;
	stati_ragg1->colore = black;
		
	return stati_ragg1;;
}

//RB TREE
//inserisce nel RB tree uno stato ragg
void inserisciStatoRagg(alberone* alberoStati1, int statoF,char in,char out,char testina)
{
	
	RBT padre,stati_ragg1;
	padre = NULL;
	stati_ragg1 = (*alberoStati1)->root;
	
	//cerca nell'albero se c'è già l'input corrispondente
	while(stati_ragg1 != NULL  &&  stati_ragg1->input != in)
	{
		padre = stati_ragg1;
		
		if(stati_ragg1->input < in)	stati_ragg1 = stati_ragg1->dx;
		
		else	stati_ragg1 = stati_ragg1->sx;
	}
	
	//se l'input è già presente utilizzo il puntatore centro
	if(stati_ragg1 != NULL  &&  stati_ragg1->input == in)
	{
		while(stati_ragg1 != NULL)
		{
			padre = stati_ragg1;
			stati_ragg1 = stati_ragg1->centro;
		}
		
		stati_ragg1 = malloc(sizeof(stati_ragg));
		stati_ragg1->input = in;
		stati_ragg1->dest = statoF;
		stati_ragg1->output = out;
		stati_ragg1->testina = testina;
		stati_ragg1->dx = NULL;
		stati_ragg1->sx = NULL;
		stati_ragg1->centro = NULL;
		stati_ragg1->padre = padre;
		padre->centro = stati_ragg1;
	}
	
	//input non presente nell'albero
	else
	{
		void bilanciaStatiRagg(RBT* nodo);
		
		stati_ragg1 = malloc(sizeof(stati_ragg));
		stati_ragg1->input = in;
		stati_ragg1->dest = statoF;
		stati_ragg1->output = out;
		stati_ragg1->testina = testina;
		stati_ragg1->dx = NULL;
		stati_ragg1->sx = NULL;
		stati_ragg1->centro = NULL;
		stati_ragg1->padre = padre;
		
		if(padre->input < in)	padre->dx = stati_ragg1;
		
		else	padre->sx = stati_ragg1;
		
		bilanciaStatiRagg(&stati_ragg1);
		
		while( (*alberoStati1)->root->padre != NULL)
		{
			(*alberoStati1)->root = (*alberoStati1)->root->padre;
		}
	}
	return;
}



//RB TREE
//effetua una rotazione dx o sx nell'albero RB
void ruotaSR(char lato,RBT* root)
{
	RBT padre,figlio;
	figlio = NULL;
	padre = (*root)->padre;
	
	if(lato == 's')
	{
		figlio = (*root)->dx;
		(*root)->dx = figlio->sx;
		
		if(figlio->sx != NULL)	figlio->sx->padre = *root;
		
		figlio->sx = *root;
		(*root)->padre = figlio;
		figlio->padre = padre;
		
		if(padre != NULL)
		{
			if(padre->sx == *root)	padre->sx = figlio;
			
			else	padre->dx = figlio;
		}
	}
	
	else if(lato == 'd')
	{
		figlio = (*root)->sx;
		(*root)->sx = figlio->dx;
		
		if(figlio->dx != NULL)	figlio->dx->padre = *root;
		
		figlio->dx = *root;
		(*root)->padre = figlio;
		figlio->padre = padre;
		
		if(padre != NULL)
		{
			if(padre->dx == *root)	padre->dx = figlio;
			
			else	padre->sx = figlio;
		}
	
	}
	
	return;
}



//RB TREE
//bilancia albero RB
void bilanciaStatiRagg(RBT* nodo)
{
	void ruotaSR(char lato,RBT* root);
	RBT stati_ragg1 = *nodo;
	stati_ragg1->colore = red;
	
	while(stati_ragg1 != NULL)
	{
		RBT padre = stati_ragg1->padre;
		RBT nonno,zio;
		nonno = NULL;
		zio = NULL;
		
		if(padre != NULL) nonno = padre->padre;
		
		if(nonno != NULL) 
		{
			if(nonno->sx == padre)	zio = nonno->dx;
			else	zio = nonno->sx;
		}
		
		
		//CASO1: se non ha padre il nodo è radice, coloro di nero e termino
		if(padre == NULL)
		{
			stati_ragg1->colore = black;
			stati_ragg1 = NULL;
		}
		
		//CASO2:padre nero
		else if(padre->colore == black)	stati_ragg1 = NULL;
		
		//CASO3:zio rosso
		else if(zio != NULL  &&  zio->colore == red)
		{
			padre->colore = black;
			zio->colore = black;
			nonno->colore = red;
			stati_ragg1 = nonno;
		}
		
		else
		{
			if(stati_ragg1 == padre->dx  &&  padre == nonno->sx)	//CASO4a:
			{
				ruotaSR('s',&padre);
				stati_ragg1 = padre;
			}
			
			else if(stati_ragg1 == padre->sx  &&  padre == nonno->dx)	//CASO4b:
			{
				ruotaSR('d',&padre);
				stati_ragg1 = padre;
			}
			
			else
			{
				if(stati_ragg1 == padre->sx  &&  padre == nonno->sx)	ruotaSR('d',&nonno);	//CASO5a:
				
				else if(stati_ragg1 == padre->dx  &&  padre == nonno->dx)	ruotaSR('s',&nonno);	//CASO5b:
			
				padre->colore = black;
				nonno->colore = red;
				stati_ragg1 = NULL;
			}
		}	
	}
	
	return;
}

//RB TREE
//visita l'albero degli stati raggiungibili cercando un elemento
RBT cercaStatiRagg(RBT* root, char ricercato)
{

	RBT cursoreStatiRagg = *root;
	
	while(cursoreStatiRagg != NULL  &&  cursoreStatiRagg->input != ricercato)
	{
		if(cursoreStatiRagg->input < ricercato)	cursoreStatiRagg = cursoreStatiRagg->dx;
		
		else	cursoreStatiRagg = cursoreStatiRagg->sx;
	}

	return cursoreStatiRagg;
}



//---------
//-LETTURA-
//---------
//controlla che l'in inizi con tr
int checkTr()
{
	
	char t,r;
	
	scanf(" %c",&t);
	scanf(" %c",&r);
	
	return 0;
}



//LETTURA
//legge una riga di tr, se ne legge effettivamente
//una allora ritorna 1, se legge acc ritorna 0.
//Variabili passate per indirizzo per poterle 
//riutilizzare nel main. 
int leggiRiga(int* statoI,char* in,char* out,char* testina,int* statoF)
{
	int  statoI1,statoF1;
	char in1,out1,testina1,carattere = '_';
	
	scanf(" %c",&carattere);
	
	if('a' == carattere)	
	{
		scanf("%c",&carattere);
		scanf("%c",&carattere);

		return 0;
	}
	
	statoI1 = carattere-48;
	scanf("%c",&carattere);
	
	while(' ' != carattere)
	{
		statoI1 = (statoI1*10)+carattere-48;
		scanf("%c",&carattere);
	}
	
	scanf(" %c",&in1);
	scanf(" %c",&out1);
	scanf(" %c",&testina1);
	scanf("%d",&statoF1);
	
	*statoI = statoI1;
	*in = in1;
	*out = out1;
	*testina = testina1;
	*statoF = statoF1;
	
	return 1;
}


//LISTA
//lista bidirezionale con sentinella contenente gli stati di accettazione
lista leggiAcc(alberone* alberoStati)
{
	void inserisciStati(alberone* alberoStati, int statoI,char in, char out,char testina,int statoF,int specialCase);
	
	char carattere = '_';	
	lista accettati = malloc(sizeof(elemento));
	lista sentinella = malloc(sizeof(elemento));
	
	//inizializzo il primo stato di acc e la sentinella
	sentinella->statoAcc = -1;
	sentinella->succ = accettati;
	sentinella->prec = accettati;
	
	accettati->prec = sentinella;
	accettati->succ = sentinella;
	scanf("%d",&(accettati->statoAcc)); //scansione stato accettazione
	inserisciStati( &(*alberoStati),accettati->statoAcc,'_','_','_',-1,1);//inserisco il nuovo acc nell'alberone degli stati
	
	scanf(" %c",&carattere);
	
	//leggo caratteri fino a quando non trovo 'm' di max
	while('m' != carattere)
	{
		lista accTemp = malloc(sizeof(elemento));
		
		accTemp->statoAcc = carattere - 48;
		scanf("%c",&carattere);
	
		//continuo a leggere lo stato di acc carattere per carattere
		while('\n' != carattere)
		{
			accTemp->statoAcc = (accTemp->statoAcc * 10) + carattere - 48;
			scanf("%c",&carattere);
		}
		
		//inserisco il nuovo stato di acc nell'alberone degli stati
		inserisciStati( &(*alberoStati),accTemp->statoAcc,'_','_','_',-1,1);
		
		//collego il nuovo elemento in fondo alla lista
		accTemp->prec = sentinella->prec;
		accTemp->succ = sentinella;
		sentinella->prec->succ = accTemp;
		sentinella->prec = accTemp;
		
		scanf("%c",&carattere);
	}
	
	return accettati;
}



//LETTURA
//legge "max",massimo numero passi e "run"
unsigned int leggiMax()
{
	char maxStr[L_STR],run[L_STR],acapo;
	unsigned int maxTransizioni1;
	
	scanf("%c",&acapo);
	fgets(maxStr,L_STR,stdin);
	scanf("%u",&maxTransizioni1);
	scanf("%c",&acapo);
	fgets(run,L_STR,stdin);

	return maxTransizioni1;
}

//--------
//-NASTRO-
//--------
//inizializza il nastro con il primo carattere dell'input e una sentinella
nastro creaNastro()
{
	char car;
	int risScan;
	nastro nastro1,sentinella;
	nastro1 = malloc(sizeof(slot));
	sentinella = malloc(sizeof(slot));
	risScan = scanf(" %c",&car);
	
	if(EOF != risScan)
	{
		nastro1->input = car;
		nastro1->succ = sentinella;
		nastro1->prec = sentinella;
		sentinella->input = '_';
		sentinella->succ = nastro1;
		sentinella->prec = nastro1;
	}
	
	else	nastro1 = NULL;
	
	return nastro1;

}


//LETTURA e NASTRO
/*Ricorsiva, inserisce nel nastro passato per indirizzo i vari caratteri dell'input*/
int leggiInput(nastro* nastroIn)
{
	char car = '_';
	int risScan = scanf("%c",&car);
	
	if(EOF == risScan)	return 0;
	
	else if('\n' == car)	return 1;
	
	else 
	{
		nastro ultimo,sentinella,nuovo;
		nuovo = malloc(sizeof(slot));
		
		ultimo = (*nastroIn)->prec->prec;
		sentinella = (*nastroIn)->prec;
		
		if (sentinella->input != '_') return -1;
		
		nuovo->input = car;
		nuovo->succ = sentinella;
		nuovo->prec = ultimo;
		ultimo->succ = nuovo;
		sentinella->prec = nuovo;
	}
	
	return leggiInput( &(*nastroIn));
}

//NASTRO
//
void tagliaNastro(nastro* nastroIn)
{
	nastro cursoreNastro = *nastroIn;
	nastro nuovo=malloc(sizeof(slot));
	
	cursoreNastro->prec->succ = NULL;
	nuovo->input = '_';
	nuovo->prec = NULL;
	nuovo->succ = cursoreNastro;
	cursoreNastro->prec = nuovo;
	
	return;
}


//-----------------------
//-LETTURA, NASTRO, TEST-
//-----------------------
//funzione che chiama crea nastro, leggi input e le funzioni per testare l'input nella TM (che non ho ancora scritto).L'utilità di run() è non dover fare le free
// del nastro ogni volta che testo un altro input, operazione che avrei dovuto fare se queste funzioni le avessi chiamate dal main
int run(alberone* alberoStati,unsigned int* maxTransizioni, lista* statiAcc)
{
	int richiama;
	nastro nastroIn;
	
	nastro creaNastro();
	int leggiInput(nastro* nastroIn);
	alberone cercaStati(alberone* alberoStati, int ricercato);
	void tagliaNastro(nastro* nastroIn);
	void liberaNastro(nastro nastroIn);
	void testaInput(nastro *cursoreNastro, RBT *cursoreStatiRagg,alberone* cursoreAlberone,unsigned int*numeroTransizioni, 
					int* accetta, lista* statiAcc, unsigned int* maxTransizioni);
	
		
	nastroIn = creaNastro();
	
	//le righe vuote non sono da testare
	if(nastroIn == NULL)
	{
		richiama = 0;
		return richiama;
	}
	
	richiama = leggiInput(&nastroIn);
	int accetta = 2;
	unsigned int numeroTransizioni = 0;
	alberone cursoreAlberone = cercaStati( &(*alberoStati),0);
	//alberone radiceAlberone = *alberoStati;
	RBT cursoreStatiRagg;
	lista cursoreLista = *statiAcc;
	
	while(nastroIn->prec->input != '_')	nastroIn = nastroIn->prec;
	
	nastro cursoreNastro = nastroIn;
	
	tagliaNastro( &cursoreNastro);
	
	printf("inizio test input\n");
	
	testaInput( &cursoreNastro, &cursoreStatiRagg, &cursoreAlberone, &numeroTransizioni, &accetta, &cursoreLista,
				&(*maxTransizioni));

	printf("finito test input\n");
	
	if(1 == accetta)	printf("1\n");
	
	else if(0 == accetta)	printf("0\n");
	
	else if(-1 == accetta)	printf("U\n");	

	
	//riporta il nastro all'inizio per sicurezza
	while(nastroIn->prec != NULL)	nastroIn = nastroIn->prec;
	
	//libera memoria nastro
	liberaNastro(nastroIn);
	
	return richiama;
}

//!!!DEVI chiamare gestisciIstantanee anche quando *accetta = -1!!!!
//TEST
//
void testaInput(nastro *cursoreNastro, RBT *cursoreStatiRagg,alberone* cursoreAlberone,unsigned int*numeroTransizioni, 
				int* accetta, lista* statiAcc, unsigned int* maxTransizioni)
{
	int controllaStati(lista statiAcc, alberone* cursoreAlberone);
	void eseguiTransizione(nastro *cursoreNastro, RBT *cursoreStatiRagg,alberone* cursoreAlberone,int* accetta);
	int controllaNumero(unsigned int numeroTransizioni, unsigned int maxTransizioni, int* accetta);
	RBT cercaStatiRagg(RBT* root, char ricercato);
	listaIstantanee salvaIstantanea(listaIstantanee backup, nastro cursoreNastro, alberone cursoreAlberone, 									unsigned int numeroTransizioni);
	
	int gestisciIstantanee(nastro* cursoreNastro, alberone* cursoreAlberone, RBT* cursoreStatiRagg, listaIstantanee backup, 								unsigned int *numeroTransizioni);
	
	int i= 0, ricorda = 0, ripristinato = 0;
	listaIstantanee backup = NULL;
	*cursoreStatiRagg = cercaStatiRagg( &( (*cursoreAlberone)->root), (*cursoreNastro)->input);
 	RBT cursoreStatiRagg1;
 	
	do
	{
		cursoreStatiRagg1 = *cursoreStatiRagg;
		if (ripristinato == 0)	cursoreStatiRagg1 = cercaStatiRagg( &( (*cursoreAlberone)->root), (*cursoreNastro)->input);
 		
 		if(NULL == cursoreStatiRagg1)	
		{
			*accetta = controllaStati( (*statiAcc), &(*cursoreAlberone));
			
			if(0 == *accetta  ||  -1 == *accetta)
			{
				if (!gestisciIstantanee( &(*cursoreNastro), &(*cursoreAlberone), &(*cursoreStatiRagg), backup, 											 &(*numeroTransizioni)))
				{
					*accetta = 0;
					return;
				}
					
				ripristinato = 1;
				*accetta = 2;
			}
		} 
 		
 		//DETERMINISMO
		while(cursoreStatiRagg1 != NULL  &&  cursoreStatiRagg1->centro == NULL  &&  *accetta != -1  &&  *accetta != 0  &&  				  *accetta != 1)
		{
			printf("determinismo, inizio a eseguire la transizione\n");
		
			//su statiRagg1
			eseguiTransizione( &(*cursoreNastro), &(cursoreStatiRagg1), &(*cursoreAlberone),&(*accetta) ); 
			
			if (ripristinato == 1)	ripristinato = 0;		
			
			(*numeroTransizioni)++;
 			
 			ricorda = controllaNumero(*numeroTransizioni, *maxTransizioni, &(*accetta));
			if (ripristinato == 0)	cursoreStatiRagg1 = cercaStatiRagg(&((*cursoreAlberone)->root), (*cursoreNastro)->input);
 			
 			//se la macchina si blocca
			if(NULL == cursoreStatiRagg1)	
			{
				*accetta = controllaStati( (*statiAcc), &(*cursoreAlberone));
				
				if(0 == *accetta  ||  -1 == *accetta)
				{
					if (!gestisciIstantanee( &(*cursoreNastro), &(*cursoreAlberone), &(*cursoreStatiRagg), backup, 														&(*numeroTransizioni)))
					{
						*accetta = 0;
						return;
					}
					
					ripristinato = 1;
					*accetta = 2;
				}
			} 
		
		}
		
		//NON DETERMINISMO
		if( (cursoreStatiRagg1 != NULL  &&  cursoreStatiRagg1->centro != NULL)  ||  *accetta == -1  ||  *accetta == 0)
		{
			printf("non determinismo\n");
			//
					
			backup = salvaIstantanea(backup, *cursoreNastro, *cursoreAlberone, *numeroTransizioni);
			
			//su statiRagg
			eseguiTransizione( &(*cursoreNastro), &(*cursoreStatiRagg), &(*cursoreAlberone), &(*accetta) );
			if (ripristinato == 1)	ripristinato = 0;
			
			(*numeroTransizioni)++;
			
			ricorda = controllaNumero(*numeroTransizioni, *maxTransizioni, &(*accetta));
			if (ripristinato == 0)	*cursoreStatiRagg = cercaStatiRagg( &((*cursoreAlberone)->root), (*cursoreNastro)->input);
	
			if(NULL == *cursoreStatiRagg)	
			{
				*accetta = controllaStati( (*statiAcc), &(*cursoreAlberone));
				
				if(0 == *accetta  ||  -1 == *accetta)
				{
					if (!gestisciIstantanee( &(*cursoreNastro), &(*cursoreAlberone), &(*cursoreStatiRagg), backup, 														&(*numeroTransizioni)))
					{
						*accetta = 0;
						return;
					}
					
					ripristinato = 1;
					*accetta = 2;
				}
			} 
		
		}
 		
 		if( 1 == ricorda  &&  *accetta == 0)	*accetta = -1;
 		
 		i++;
 	
	}while(*cursoreStatiRagg != NULL  &&  *accetta != 1  &&  i < 100);
 	
}

//aggiungere il controllo della U
//TEST
//controlla se lo stato in cui si è bloccata è di accettazione
int controllaStati(lista statiAcc, alberone* cursoreAlberone)
{
	lista cursoreLista = statiAcc;
	
	printf("controlla stati\n");
	
	//mi assicuro di essere in cima alla lista
	while(cursoreLista->prec->statoAcc != -1)
	{	
		if(cursoreLista->statoAcc == -1)	cursoreLista = cursoreLista->succ;
		
		else	cursoreLista = cursoreLista->prec;
	}
	
	//cerco lo stato corrente nella lista
	while(cursoreLista->statoAcc != -1  &&  (*cursoreAlberone)->id != cursoreLista->statoAcc)
	{
		cursoreLista = cursoreLista->succ;
	}
		
	//accetta
	if( (*cursoreAlberone)->id == cursoreLista->statoAcc)		return 1;
	
	//non accetta
	else		return 0;
}



//TEST
//controlla numero transizioni
int controllaNumero(unsigned int numeroTransizioni, unsigned int maxTransizioni, int* accetta)
{
	if(numeroTransizioni > maxTransizioni)
	{
		*accetta = -1;
		return 1;
	}
	
	else	return 0;
}



//TEST
//
int gestisciIstantanee(nastro* cursoreNastro, alberone* cursoreAlberone, RBT* cursoreStatiRagg, listaIstantanee backup, 							unsigned int *numeroTransizioni)
{
	void liberaNastro(nastro nastroIn);
	int ripristinaIstantanea(listaIstantanee backup, nastro* cursoreNastro, alberone* cursoreAlberone, 								 unsigned int *numeroTransizioni);
	listaIstantanee eliminaIstantanea(listaIstantanee backup);
	RBT cercaStatiRagg(RBT* root, char ricercato);
	
	int salta;
	
	//*accetta = 2;
	printf("gestisciIstantanee\n");
	nastro temp1 = *cursoreNastro;
	//temp1->succ = NULL;????
	liberaNastro(temp1);
	salta = ripristinaIstantanea(backup, &(*cursoreNastro), &(*cursoreAlberone), &(*numeroTransizioni));
	
	if(salta == -1) return 0;
	
	*cursoreStatiRagg = cercaStatiRagg( &((*cursoreAlberone)->root), (*cursoreNastro)->input);
	
	if(*cursoreStatiRagg == NULL) 
	{
		printf("OOOOOOOOPS!!\n");
		backup = eliminaIstantanea(backup);
		return gestisciIstantanee( &(*cursoreNastro), &(*cursoreAlberone), &(*cursoreStatiRagg), backup, 									   &(*numeroTransizioni));
	}
	
	while(salta > 0  &&  (*cursoreStatiRagg)->centro != NULL)
	{
		*cursoreStatiRagg = (*cursoreStatiRagg)->centro;
		salta--;
	}
	
	
	if( (*cursoreStatiRagg)->centro == NULL)	backup = eliminaIstantanea(backup);
	
	return 1;
}



//TEST
//
listaIstantanee salvaIstantanea(listaIstantanee backup, nastro cursoreNastro, alberone cursoreAlberone, 								unsigned int numeroTransizioni)
{
	nastro copiaNastro(nastro cursoreNastro);
	printf("salva istantanea\n");
	if(backup == NULL)
	{
		backup = malloc(sizeof(istantanea));
		backup->buPosizione = copiaNastro(cursoreNastro);
		backup->buStato = cursoreAlberone->id;
		backup->salti = 0;
		backup->numeroTransizioni = numeroTransizioni;
		backup->succ = NULL;
	}
	
	else
	{
		int comparaNastro(nastro nastro1,nastro nastro2);
	
		listaIstantanee nuovo = malloc(sizeof(istantanea));
		nuovo->buPosizione = copiaNastro(cursoreNastro);
		nuovo->buStato = cursoreAlberone->id;
		nuovo->numeroTransizioni = numeroTransizioni;
		nuovo->salti = 0;
		
		if(nuovo->buStato != backup->buStato  ||  nuovo->numeroTransizioni != backup->numeroTransizioni  ||  		            		   !comparaNastro(nuovo->buPosizione, backup->buPosizione))
		{ 
			nuovo->succ = backup;
			backup = nuovo;
		}
	}
	
	return backup;
}




//TEST
//compara i nastri
int comparaNastro(nastro nastro1, nastro nastro2)
{
	if (nastro1->input == nastro2->input)
	{
		while(nastro1->prec != NULL  &&  nastro2->prec != NULL)
		{
			nastro1 = nastro1->prec;
			nastro2 = nastro2->prec;
			
			if((nastro1->prec == NULL  &&  nastro2->prec != NULL)  ||  (nastro2->prec == NULL  &&  nastro1->prec != NULL))
				return 0;
		}
		
		while (nastro1->succ != NULL  &&  nastro2->succ != NULL  &&  nastro1->input == nastro2->input)
		{
			nastro1 = nastro1->succ;
			nastro2 = nastro2->succ;
		}
		
		if(nastro1->input != nastro2->input)	return 0;
		
		else	return 1;
	}
	
	else	return 0;
}


//TEST
//
listaIstantanee eliminaIstantanea(listaIstantanee backup)
{
	printf("elimina istantanea\n");
	listaIstantanee temp = backup;
	if(backup->succ != NULL)	backup = backup->succ;
	temp->succ = NULL;
	free(temp);

	return backup;
}



//TEST
//
int ripristinaIstantanea(listaIstantanee backup, nastro* cursoreNastro,alberone* cursoreAlberone, unsigned int* numeroTransizioni)
{
	if(backup == NULL)	return -1;
	
	alberone cercaStati(alberone* alberoStati, int ricercato);
	
	while((*cursoreAlberone)->padre != NULL)	*cursoreAlberone = (*cursoreAlberone)->padre;
	
	*cursoreNastro = backup->buPosizione;
	*cursoreAlberone = cercaStati( &(*cursoreAlberone), backup->buStato);
	*numeroTransizioni = backup->numeroTransizioni;
	backup->salti++;
	
	return backup->salti;

}

//TEST
//effettua una transizione
//DA CERCARE
void eseguiTransizione(nastro *cursoreNastro, RBT *cursoreStatiRagg,alberone* cursoreAlberone, int* accetta)
{

	printf("cerco input tra le possibili transizioni, (*cursoreAlberone)->id=%d, (*cursoreNastro)->input=%c\n",(*cursoreAlberone)->id,(*cursoreNastro)->input);
	
	nastro cursoreNastro1 = *cursoreNastro;
	//modifica nastro se necessario
	if ((*cursoreNastro)->input != (*cursoreStatiRagg)->output)
		(*cursoreNastro)->input = (*cursoreStatiRagg)->output;
	
	//sposto testina se ce n'è bisogno
	if( (*cursoreStatiRagg)->testina == 'R')	*cursoreNastro = (*cursoreNastro)->succ;

	else if( (*cursoreStatiRagg)->testina == 'L')	*cursoreNastro = (*cursoreNastro)->prec;
	
	//se supero l'ultimo blank allora devo crearne un altro e attaccarlo al nastro
	if(NULL == (*cursoreNastro)->prec  ||  NULL == (*cursoreNastro)->succ)
	{
		nastro cursoreNastro2 = malloc(sizeof(slot));
					
		if( (*cursoreStatiRagg)->testina == 'R')
		{
			cursoreNastro2->input = '_';
			cursoreNastro2->succ = NULL;
			cursoreNastro2->prec = *cursoreNastro;
			(*cursoreNastro)->succ = cursoreNastro2;
		}
		
		else if( (*cursoreStatiRagg)->testina == 'L')
		{
			cursoreNastro2->input = '_';
			cursoreNastro2->prec = NULL;
			cursoreNastro2->succ = *cursoreNastro;
			(*cursoreNastro)->prec = cursoreNastro2;
		}
	}
		
	//se ce n'è bisogno cambio stato
	if( (*cursoreAlberone)->id != (*cursoreStatiRagg)->dest)
	{
		while( (*cursoreAlberone)->padre != NULL)	*cursoreAlberone = (*cursoreAlberone)->padre;
	
		*cursoreAlberone = cercaStati( &(*cursoreAlberone), (*cursoreStatiRagg)->dest);
		
		//se lo stato dest è uno stato pozzo allora termino 
		if(NULL == *cursoreAlberone)
		{
			 cursoreNastro1->input = (*cursoreStatiRagg)->input;
			 *accetta = 0;
			 return;
		}
	}
	
	//aggiorno cursoreStatiRagg e numeroTransizioni
	RBT cursoreStatiRagg1 = *cursoreStatiRagg;
	*cursoreStatiRagg = (*cursoreAlberone)->root;
}



//NASTRO+TEST
//
nastro copiaNastro(nastro cursoreNastro)
{
	nastro gianni = cursoreNastro;
	
	while(cursoreNastro->prec->input != '_') 	cursoreNastro = cursoreNastro->prec;

	
	nastro clone = malloc(sizeof(slot));
	nastro marcello = malloc(sizeof(slot));
	
	marcello->input = '_';
	marcello->prec = NULL;
	marcello->succ = clone;
	clone->prec = marcello;
	clone->succ = NULL;
	clone->input = cursoreNastro->input;
	marcello = clone;
	
	
	while(cursoreNastro->input != '_')
	{
		nastro temp = malloc(sizeof(slot));
			
		cursoreNastro = cursoreNastro->succ;
		temp->input = cursoreNastro->input;
		temp->succ = NULL;
		temp->prec = marcello;
		marcello->succ = temp;
		marcello = temp;
	
		if(cursoreNastro == gianni)	clone = marcello;
	}
	
	return clone;
}


//FREE
//
void liberaAlberone(alberone alberoStati1)
{
	if(alberoStati1 != NULL)
	{
		void liberaStatiRagg(RBT statiRagg1);
	
		if(alberoStati1->sx != NULL  ||  alberoStati1->dx != NULL)
		{	
			RBT temp = alberoStati1->root;
			alberoStati1->root = NULL;
			liberaStatiRagg(temp);
			liberaAlberone(alberoStati1->sx);
			
			liberaAlberone(alberoStati1->dx);
		}
		
		if(alberoStati1->sx == NULL  &&  alberoStati1->dx == NULL)
		{
			
			if(alberoStati1->padre != NULL)
			{
				RBT temp1 = alberoStati1->root;
				alberoStati1->root = NULL;
				liberaStatiRagg(temp1);
				
				alberone temp = alberoStati1;
				alberoStati1 = alberoStati1->padre;
			
				if (alberoStati1->sx == temp)	alberoStati1->sx = NULL;
			
				else	alberoStati1->dx = NULL;
			
				free(temp);
			}
			
			else	free(alberoStati1);
		}
	}
}


//FREE
//
void liberaStatiRagg(RBT statiRagg1)
{
	if(statiRagg1 != NULL)
	{
		if(statiRagg1->sx != NULL  ||  statiRagg1->dx != NULL  ||  statiRagg1->centro != NULL)
		{
			liberaStatiRagg(statiRagg1->sx);
			liberaStatiRagg(statiRagg1->centro);
			liberaStatiRagg(statiRagg1->dx);
		}

		if(statiRagg1->sx == NULL  &&  statiRagg1->dx == NULL  &&  statiRagg1->centro == NULL)
		{
			if(statiRagg1->padre != NULL)
			{
				RBT temp = statiRagg1;
				statiRagg1 = statiRagg1->padre;
				
				if(statiRagg1->sx == temp)	statiRagg1->sx = NULL;
				
				if(statiRagg1->dx == temp)	statiRagg1->dx = NULL;
				
				if(statiRagg1->centro == temp)	statiRagg1->centro = NULL;
				
				free(temp);
			}
			
			else	free(statiRagg1);
		}
	}
}


//FREE
//
void liberaNastro(nastro nastroIn)
{
	
	if(nastroIn->succ != NULL)
	{
		nastro temp = nastroIn;	
		nastroIn = nastroIn->succ;
		nastroIn->prec = NULL;
		free(temp);
		
		liberaNastro(nastroIn);
	}
	
	else	free(nastroIn);
}


//FREE
//
void liberaLista(lista statiAcc)
{
	if (statiAcc != NULL)
	{
		lista temp = statiAcc;
		statiAcc = statiAcc->succ;
		
		if (temp->prec != NULL)	temp->prec->succ = NULL;
		
		if (statiAcc != NULL)	statiAcc->prec = NULL;
		
		free(temp);
		liberaLista(statiAcc);
	}
}



//--------------
//-****MAIN****-
//--------------
int main()
{
	//dichiarazioni
	alberone alberoStati;
	int  statoI,statoF,continua;
	int acc;
	unsigned int maxTransizioni;
	char in,out,testina;
	lista statiAcc;
	
	
	//prototipi
	//void leggiFileInput();
	int checkTr();
	int leggiRiga(int* statoI,char* in,char* out,char* testina,int* statoF);
	void creaStati(alberone* alberoStati, int statoI,char in, char out,char testina,int statoF);
	void inserisciStati(alberone* alberoStati, int statoI,char in, char out,char testina,int statoF,int specialCase);
	unsigned int leggiMax();
	int run(alberone* alberoStati,unsigned int* maxTransizioni, lista* statiAcc);
	lista leggiAcc(alberone* alberoStati);
	void liberaAlberone(alberone alberoStati1);
	void liberaLista(lista statiAcc);
	//void stampaStati(alberone alberoStati);
	//void stampaStatiRagg(RBT root);
	
	//codice
	checkTr();

	//legge righe di tr e forma le strutture dati
	continua = leggiRiga(&statoI,&in,&out,&testina,&statoF);
	creaStati(&alberoStati,statoI,in,out,testina,statoF);
	continua = leggiRiga(&statoI,&in,&out,&testina,&statoF);

	while(1 == continua)
	{
		inserisciStati(&alberoStati,statoI,in,out,testina,statoF,0);
	
		continua = leggiRiga(&statoI,&in,&out,&testina,&statoF);
	}
	
	//legge stati di acc
	statiAcc = leggiAcc(&alberoStati);
	
	//legge max
	maxTransizioni = leggiMax();
	
	//finchè ci sono input li testa
	//do
	//{
		continua = run(&alberoStati,&maxTransizioni, &statiAcc);
		
	//}while(1 == continua);
	
	//riporto per sicurezza albero stati alla radice
	while(alberoStati->padre != NULL)	alberoStati = alberoStati->padre;
	
	//libera memoria
	liberaAlberone(alberoStati);
	liberaLista(statiAcc);
	
	return 0;
}


//da implementare: eseguiTransizione() OK, salvaIstantanea() OK, controllaStati() OK, gestisciIstantanee() OK, eliminaIstantanea()
// OK, ripristinaIstantanea() OK
