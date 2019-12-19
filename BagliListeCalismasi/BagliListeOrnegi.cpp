#serdar kocer


#include <QCoreApplication>



struct alan{
    int alanNo;
    struct alan *sonraki;
};

struct otopark{
    struct alan *bosListeBasiPtr;
    struct alan *doluListeBasiPtr;

};

extern void initializeotoparkAlanlari(struct otopark *otoparkPtr);
extern struct alan * getBosAlan(struct otopark * otoparkPtr);
extern void printBosAlanlar(struct  alan *alanInfoPtr);
extern void printDoluAlanlar(struct  alan *alanInfoPtr);
extern bool aracCikar(int alanNo, struct otopark * otoparkPtr);
extern void araciBosListesineEkleveDoluListesiniDuzenle(alan * aracAlanPtr,alan * aracAlanPtrPrev,struct otopark * otoparkPtr );
extern void initializeotoparkAlanlari(struct otopark *otoparkPtr);

#define CIKANARACSAYISI 3
#define GELENARACSAYISI 3
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    struct alan *tmpAracAlanPtr;
    struct otopark *otoparkPtr,otoparkBilgi;
    int gelenAracSayisi=GELENARACSAYISI;
    int cikanAracDizisi[]={3,1,2};
    otoparkPtr=&otoparkBilgi;
    otoparkPtr->bosListeBasiPtr=0;
    otoparkPtr->doluListeBasiPtr=0;

    initializeotoparkAlanlari(otoparkPtr);//otopark alanlari icin memory allocation yapildi.


   printBosAlanlar(otoparkPtr->bosListeBasiPtr);
   printDoluAlanlar(otoparkPtr->doluListeBasiPtr);
   int tmpAracCnt = 0;
   while(gelenAracSayisi > 0){


       tmpAracAlanPtr=getBosAlan(otoparkPtr);
       if(otoparkPtr->doluListeBasiPtr == 0){
        otoparkPtr->doluListeBasiPtr = tmpAracAlanPtr;
        }else{
         tmpAracAlanPtr->sonraki = otoparkPtr->doluListeBasiPtr;
         otoparkPtr->doluListeBasiPtr = tmpAracAlanPtr;
        }
        gelenAracSayisi--;
        tmpAracCnt++;
       printf("\n");
       printf ("---- %d. ARAC GELDI  ----",tmpAracCnt);
       printf("\n");

        printBosAlanlar(otoparkPtr->bosListeBasiPtr);
        printDoluAlanlar(otoparkPtr->doluListeBasiPtr);

   }

    for(int i=0; i<CIKANARACSAYISI;i++){
        aracCikar(cikanAracDizisi[i],otoparkPtr);
    }

    return a.exec();
}

struct alan * getBosAlan(struct otopark * otoparkPtr){
    struct alan * tempBosListeBasiPtr;

    if(otoparkPtr->bosListeBasiPtr==0){
        return 0;
    }

    tempBosListeBasiPtr = otoparkPtr->bosListeBasiPtr;
    otoparkPtr->bosListeBasiPtr = otoparkPtr->bosListeBasiPtr->sonraki;
    tempBosListeBasiPtr->sonraki = 0;
    return tempBosListeBasiPtr;

}
bool aracCikar(int alanNo, struct otopark * otoparkPtr){
    alan * aracAlanPtr = 0;
    alan * aracAlanPtrPrev = 0;
    alan * loopAracPtr = otoparkPtr->doluListeBasiPtr;

    while(loopAracPtr!=0){
        if(loopAracPtr->alanNo == alanNo){
            aracAlanPtr = loopAracPtr;
            break;
        }
        if(loopAracPtr->sonraki == 0)
            break;
        aracAlanPtrPrev = loopAracPtr;
        loopAracPtr = loopAracPtr->sonraki;
    }

    /*Arac Bulunduysa*/
    if(aracAlanPtr != 0 ){
        araciBosListesineEkleveDoluListesiniDuzenle(aracAlanPtr,aracAlanPtrPrev,otoparkPtr);
        printf("\n");
        printf ("---- %d. ARAC CIKTI  ----",alanNo);
        printf("\n");
        printBosAlanlar(otoparkPtr->bosListeBasiPtr);
        printDoluAlanlar(otoparkPtr->doluListeBasiPtr);
        return true;
    }
    return false;
}

void araciBosListesineEkleveDoluListesiniDuzenle(alan * aracAlanPtr,alan * aracAlanPtrPrev,struct otopark * otoparkPtr ){

    if(aracAlanPtr->sonraki == 0 && aracAlanPtrPrev == 0)// sadece 1 arac varsa
        otoparkPtr->doluListeBasiPtr = 0;
    else if(aracAlanPtr->sonraki != 0 && aracAlanPtrPrev == 0){ // dolu listesinin basindaki ciktiysa
            otoparkPtr->doluListeBasiPtr = aracAlanPtr->sonraki;
            aracAlanPtr->sonraki = 0;
    }else{ // listenin ortasindan bir arac ciktiysa
        aracAlanPtrPrev->sonraki = aracAlanPtr->sonraki;
        aracAlanPtr->sonraki = 0;
    }

    if(otoparkPtr->bosListeBasiPtr == 0)//bos listesi bos ise
        otoparkPtr->bosListeBasiPtr = aracAlanPtr;
    else if(otoparkPtr->bosListeBasiPtr->alanNo > aracAlanPtr->alanNo){//listenin basina ekleme durumu
        aracAlanPtr->sonraki  = otoparkPtr->bosListeBasiPtr;
        otoparkPtr->bosListeBasiPtr = aracAlanPtr;
    }else{//listenin ortasina ekleme durumu
        alan * tmpArac = otoparkPtr->bosListeBasiPtr;
        alan * tmpAracPrev=0;
        while(tmpArac->alanNo < aracAlanPtr->alanNo){
            if(tmpArac->sonraki == 0 )
                break;
            tmpAracPrev = tmpArac;
            tmpArac = tmpArac->sonraki;
        }
        aracAlanPtr->sonraki = tmpAracPrev->sonraki;
        tmpAracPrev->sonraki = aracAlanPtr;
    }


}

void initializeotoparkAlanlari(struct otopark *otoparkPtr){

    struct alan *lastAlanPtr;
    for(int i=0; i<10; i++){
        struct alan *alPtr = new alan;
       alPtr->alanNo=i+1;
       alPtr->sonraki=0;

       if(i == 0){
           otoparkPtr->bosListeBasiPtr=alPtr;
        }else{
           lastAlanPtr->sonraki = alPtr;
        }
        lastAlanPtr=alPtr;
    }
}

void printBosAlanlar(struct  alan *alanInfoPtr){
    printf("\n");
    printf ("---- Bos Alanlar  ------");
     while (alanInfoPtr!= 0 && alanInfoPtr->sonraki!=0) {
            printf("\n");
            printf ("alan No = %d",alanInfoPtr->alanNo);
            printf ("----   kendi adresi= %d  Sonraki  adres = %d",alanInfoPtr,alanInfoPtr->sonraki);
            printf("\n");
            alanInfoPtr= alanInfoPtr->sonraki;
     }
     printf("\n");
     printf ("alan No = %d",alanInfoPtr->alanNo);
     printf ("----   kendi adresi= %d  Sonraki  adres = %d",alanInfoPtr,alanInfoPtr->sonraki);
     printf("\n");

     printf ("-----------------------");
     printf("\n");
}


void printDoluAlanlar(struct  alan *alanInfoPtr){
    printf("\n");
    printf ("---- Dolu Alanlar  ------");
    printf("\n");
     while (alanInfoPtr!= 0 && alanInfoPtr->sonraki!=0) {
            printf ("alan No = %d",alanInfoPtr->alanNo);
            printf ("----   kendi adresi= %d  Sonraki  adres = %d",alanInfoPtr,alanInfoPtr->sonraki);
            printf("\n");
            alanInfoPtr= alanInfoPtr->sonraki;
     }
     if(alanInfoPtr!=0){
         printf ("alan No = %d",alanInfoPtr->alanNo);
         printf ("----   kendi adresi= %d  Sonraki  adres = %d",alanInfoPtr,alanInfoPtr->sonraki);
         printf("\n");
     }

     printf ("-----------------------");
     printf("\n");
}

