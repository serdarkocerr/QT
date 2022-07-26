#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qgis_core.h>
#include <qgis_gui.h>
#include <qgis.h>
#include <qgssymbol.h>
#include <qgslinesymbol.h>
#include <qgsrasterlayer.h>
#include <qgssinglesymbolrenderer.h>
#include <qgssimplifymethod.h>
#include <qgssimplelinematerialsettings.h>
#include <qgslinesymbollayer.h>
#include <qgsgeometry.h>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mapCanvas = new QgsMapCanvas();
    mapCanvas->setCanvasColor(Qt::white);

    playout  = new QVBoxLayout;
    playout->setContentsMargins(0,0,0,0);
    playout->addWidget(mapCanvas);

    centralWidget()->setLayout(playout);

    timer = new QTimer(this);
    trackAddUpdateTimer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdateSLOT()));
    connect(trackAddUpdateTimer,SIGNAL(timeout()),this,SLOT(trackAddUpdateTimerUpdateSLOT()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadMap()
{


    //QString currentDirtfw="D:/Qt5.12.12/workspace/Lex-Example/data/NE1_HR_LC_SR_W_DR.tif";
//    QString currentDirtfw="C:/Users/Serdar/Downloads/NE2_HR_LC_SR_W_DR/NE2_HR_LC_SR_W_DR.tif";
//    lRaster = new QgsRasterLayer(currentDirtfw,"basemap");
//    QgsProject::instance()->addMapLayer(lRaster);

    QString currentDirtfw ="D:/Qt5.12.12/workspace/MAPs/ne_10m_land/ne_10m_land.shp";
   // QString currentDirtfw ="C:/Users/Serdar/Downloads/TUR_adm/TUR_adm2.shp";
    lVectorLayer = new QgsVectorLayer(currentDirtfw,"landmarks","ogr");
    QgsProject::instance()->addMapLayer(lVectorLayer);

    /** Memory based layer**/
    qDebug()<<"map loading....  Memory based layer - begin";

//    lMemoryBased = new QgsVectorLayer("Point?crs=EPSG:4326&field=height:double&field=name:string(255)&index=yes", "Point Layer", "memory");
    lMemoryBased = new QgsVectorLayer("Point?crs=EPSG:4326&field=id:integer&field=city:string(255)&index=yes", "Point Layer", "memory");
    QgsVectorDataProvider *provider = lMemoryBased->dataProvider();
    QgsProject::instance()->addMapLayer(lMemoryBased);

    QList<QgsField> newAttributes ;
    QgsField newAtt1(QString("PLACENAME"),QVariant::String, QString(),255);
    QgsField newAtt2(QString("PLACEHEIGHT"),QVariant::Double, QString(),20);
    newAttributes.append(newAtt1);
    newAttributes.append(newAtt2);

    provider->addAttributes(newAttributes);
    QgsFields fields = provider->fields();
//    QgsFields fields  =  lMemoryBased->fields();
    QList <QgsFeature > features;

    qDebug()<<"map loading....  Memory based layer -fieldCount : " <<  fields.count();
    QgsField fi = fields.at(0);
    qDebug()<<"map loading....  Memory based layer -field[0] : " << fi.displayName() << " | type : " << fi.displayType();
    QgsField fi2 = fields.at(1);
    qDebug()<<"map loading....  Memory based layer -field[1] : " << fi2.displayName() << " | type : " << fi2.displayType();

    QgsField fi3 = fields.at(2);
    qDebug()<<"map loading....  Memory based layer -field[2] : " << fi3.displayName() << " | type : " << fi3.displayType();
    QgsField fi4 = fields.at(3);
    qDebug()<<"map loading....  Memory based layer -field[3] : " << fi4.displayName() << " | type : " << fi4.displayType();
    lMemoryBased->updateFields();

    QgsFeature feature;
    feature.setGeometry(QgsGeometry::fromWkt("POINT (2.2945 48.8582)"));
    feature.setFields(fields);
//    feature.setAttribute(QString("PLACEHEIGHT"), 301);
    QgsAttributes attrsPr;
    attrsPr.append(1);
    attrsPr.append(QString("Paris"));
    attrsPr.append(QString("Eiffel Tower"));
    attrsPr.append(301);
    feature.setAttributes(attrsPr);
    feature.setId(1);
//    bool isPlaceNameSetF =  feature.setAttribute(QString("PLACENAME"),QString("Eiffel Tower"));
//    qDebug()<<"isPlaceNameSetF " << isPlaceNameSetF;
    features.append(feature);
    qDebug()<<"serdar 1";

    QgsFeature feature2;
    feature2.setGeometry(QgsGeometry::fromWkt("POINT (0.0761 51.5081)"));
    feature2.setFields(fields);
//    feature2.setAttribute(QString("PLACEHEIGHT"), 27);
//    bool isPlaceNameSetF2 =  feature2.setAttribute(QString("PLACENAME"),QString("Tower of London"));
//    qDebug()<<"isPlaceNameSetF2 " << isPlaceNameSetF2;
    QgsAttributes attrsLd;
    attrsLd.append(2);
    attrsLd.append(QString("London"));
    attrsLd.append(QString("Tower of London"));
    attrsLd.append(27);
    feature2.setAttributes(attrsLd);
    feature2.setId(2);
    features.append(feature2);
    qDebug()<<"serdar 2";

    QgsFeature feature3;
    feature3.setGeometry(QgsGeometry::fromWkt("POINT (10.3964 43.7231)"));
    feature3.setFields(fields);
//    feature3.setAttribute(QString("PLACEHEIGHT"), 56);
//    bool isPlaceNameSetF3 =  feature3.setAttribute(QString("PLACENAME"),QString("Leaning Tower of Pisa"));
//    qDebug()<<"isPlaceNameSetF3 " << isPlaceNameSetF3;
    QgsAttributes attrsRm;
    attrsRm.append(3);
    attrsRm.append(QString("Roma"));
    attrsRm.append(QString("Leaning Tower of Pisa"));
    attrsRm.append(56);
    feature3.setAttributes(attrsRm);
    feature3.setId(3);
    features.append(feature3);
    qDebug()<<"serdar 3";

    bool isFeatureAdded =  provider->addFeatures(features);
    qDebug()<<"map loading....  isFeatureAdded : " << isFeatureAdded;
    qDebug()<<"map loading....  Provider capabilitiesString : " << provider->capabilitiesString();
    //provider->changeAttributeValues()
    lMemoryBased->updateExtents();
    lMemoryBased->commitChanges();
    qDebug()<<"serdar 4";


    QList <QgsMapLayer *> layers;
    layers.append(lMemoryBased);
    layers.append(lVectorLayer);
    mapCanvas->setLayers(layers);
    mapCanvas->setVisible(true);
    //mapCanvas->setExtent(QgsRectangle(-127.7,24.4,-79.3,49.1));//long lat
    mapCanvas->setExtent(QgsRectangle(25.8,39.7,30.7,41.7));//long lat
    mapCanvas->refresh();

    qDebug()<<"map loading....  Memory based layer - end";

    timer->start(1000);//1 sec timer
    trackAddUpdateTimer->start(5000);//10 sec

}

void MainWindow::timerUpdateSLOT()
{
    qDebug()<<"timerUpdateSLOT....- begin";

      QgsFeatureIterator featIt =   lMemoryBased->dataProvider()->getFeatures();

      long long  cnt = lMemoryBased->featureCount();
      qDebug()<<"timerUpdateSLOT..FEATURE cnt: "<< cnt;


    if(cnt > 0){
//        for (int i  = 0 ; i < cnt; i++){
//            qDebug()<<"timerUpdateSLOT..FEATURE ....- begin";
//            QgsFeature feature;
//            QString pNAme = feature.attribute("placeName").toString();
//            qDebug()<<"timerUpdateSLOT.. | placeName : " << pNAme;
//            //int height = feature.attribute("height").toInt();
//            qDebug()<<"timerUpdateSLOT..FEATURE ....- end";
//        }

//        QgsFeature feature = lMemoryBased->getFeature(0);
//        QgsAttributes attrbutes  = feature.attributes();
//        QString pNAme  = attrbutes.at(0).toString();
//        qDebug()<<"timerUpdateSLOT.. | placeName : " << pNAme;
//        //int height = feature.attribute("height").toInt();
//        qDebug()<<"timerUpdateSLOT..FEATURE ....- end";

         QgsFeature feature ;
         for(int i = 0; i < cnt ; i++){
             featIt.nextFeature(feature);
             if(feature.isValid()){
                 qDebug()<<"timerUpdateSLOT..FEATURE ....- is VALID";
                 int attCnt = feature.attributeCount();
                 if(attCnt > 0){
                     qDebug()<<"timerUpdateSLOT..FEATURE....- Attribute Count is grater than zero : " << attCnt;
                     QString pName = feature.attribute("PLACENAME").toString();
                     double height = feature.attribute("PLACEHEIGHT").toDouble();
                     quint64 id = feature.id();
                     if(pName.isEmpty()){
                         qDebug()<<"timerUpdateSLOT..FEATURE ....- placeName is EMPTY";

                     }else {
                         qDebug()<<"timerUpdateSLOT..FEATURE ....- placeName is  " << pName;
                         qDebug()<<"timerUpdateSLOT..FEATURE ....- height is  " << height;
                         qDebug()<<"timerUpdateSLOT..FEATURE ....- id  is  " << id;

                     }
     //               QgsAttributes attributest =  feature.attributes();
     //                for(int i = 0; i<attributest.size(); i++ ){
     //                    QVariant atrr = attributest.at(i);
     //                        int userType = atrr.userType();
     //                         qDebug()<<"timerUpdateSLOT..QVariant ....- userType : " << userType;
     //                         if(userType == QMetaType::QString){
     //                            QString plName = atrr.toString();
     //                            qDebug()<<"timerUpdateSLOT..QVariant ....- userType is QString :  " << plName;

     //                         }else if(userType == QMetaType::Double){
     //                             double heig = atrr.toDouble();
     //                             qDebug()<<"timerUpdateSLOT..QVariant ....- userType is Double :  " << heig;

     //                         }else {
     //                             qDebug()<<"timerUpdateSLOT..QVariant ....- userType is different from QString or Double ";

     //                         }
     //                }
                 }else {
                     qDebug()<<"timerUpdateSLOT..FEATURE ....- thre is NO ATTRIBUTE";
                 }
             }else {
                 qDebug()<<"timerUpdateSLOT..FEATURE ....- is INVALID";
             }

         }



    }
//    while(featIt.nextFeature(feature)){
//        qDebug()<<"timerUpdateSLOT..FEATURE ....- begin";

//        QString placeName = feature.attribute("placeName").toString();
//        int height = feature.attribute("height").toInt();
//        qDebug()<<"timerUpdateSLOT.. | placeName : " << placeName << " | height : "<<height;

//        qDebug()<<"timerUpdateSLOT..FEATURE ....- end";


//    }

    qDebug()<<"timerUpdateSLOT....- end";
}

void MainWindow::trackAddUpdateTimerUpdateSLOT()
{
    qDebug()<<"trackAddUpdateTimerUpdateSLOT....- begin";
    timerCounter++;
    QgsVectorDataProvider *provider = lMemoryBased->dataProvider();
    QgsFields fields = provider->fields();

    if(timerCounter == 1){ //ADD
        qDebug()<<"trackAddUpdateTimerUpdateSLOT....- ADD";

        QgsFeature feature;
        feature.setGeometry(QgsGeometry::fromWkt("POINT (28.9969 40.7907)"));//long lat
        feature.setFields(fields);
    //    feature.setAttribute(QString("PLACEHEIGHT"), 301);
        QgsAttributes attrsPr;
        attrsPr.append(featureIdCnt);//id
        attrsPr.append(QString("Marmara"));//city
        attrsPr.append(QString("Adalar ") /*+ QString(featureIdCnt)*/);//PLACENAME
        attrsPr.append(featureIdCnt);//height
        feature.setAttributes(attrsPr);
        feature.setId(featureIdCnt);

        lMemoryBased->dataProvider()->addFeature(feature);
//        lMemoryBased->commitChanges();
        lMemoryBased->updateFields();
        mapCanvas->refresh();


        featureIdCnt++;

    }else if(timerCounter > 1 && timerCounter < 4){ //UPDATE //20sec
        qDebug()<<"trackAddUpdateTimerUpdateSLOT....- UPDATE";

         QgsGeometry updatedGeomtry;
        if (timerCounter == 2){
             updatedGeomtry = QgsGeometry::fromWkt("POINT (29.0437 40.7428)");
        }else {
             updatedGeomtry = QgsGeometry::fromWkt("POINT (29.1371 40.7158)");
        }

//        QgsFeatureIterator featureIt = lMemoryBased->getFeatures();
        QgsFeatureIterator featureIt = provider->getFeatures();

        if(featureIt.isValid()){
            QgsFeature foundFeature;
            while(featureIt.nextFeature(foundFeature)){
                if(foundFeature.attribute("id").toInt() == (featureIdCnt - 1)){
                    qDebug()<<"trackAddUpdateTimerUpdateSLOT UPDATE....- feature id FOUND and updated.";

    //                foundFeature.setGeometry(QgsGeometry::fromWkt("POINT (28.9969 40.7907)"));
    //                  QgsGeometry updatedGeomtry = QgsGeometry::fromWkt("POINT (28.9969 40.7907)");
                    //lMemoryBased->dataProvider()->changeGeometryValues()
                    //lMemoryBased->changeGeometry(foundFeature.id(),updatedGeomtry);
                    QgsGeometryMap mGeo;
                    mGeo.insert(foundFeature.id(),updatedGeomtry);
                    lMemoryBased->dataProvider()->changeGeometryValues(mGeo);
                    lMemoryBased->commitChanges();
                    lMemoryBased->updateFields();
                    mapCanvas->refresh();


                }

           }
        }else {
             qDebug()<<"trackAddUpdateTimerUpdateSLOT UPDATE....- feature ids not valid.NOT FOUND";
        }

    }else{ //DELETE
        qDebug()<<"trackAddUpdateTimerUpdateSLOT....- DELETE";

//        QgsFeatureIds ids;
//        QgsFeatureId id = featureIdCnt - 1;
//        ids.insert(id);

//         QgsFeatureIterator featureIt = lMemoryBased->getFeatures(ids);
//        if(featureIt.isValid()){
//             QgsFeature foundFeature;
//             if (featureIt.nextFeature(foundFeature)){
//                  lMemoryBased->deleteFeature(foundFeature.id());
//             }
//        }
        QgsFeatureIterator featureIt = lMemoryBased->getFeatures();
        if(featureIt.isValid()){
             QgsFeature foundFeature;
              while(featureIt.nextFeature(foundFeature)){
                 if(foundFeature.attribute("id").toInt() == (featureIdCnt - 1)){
                     qDebug()<<"trackAddUpdateTimerUpdateSLOT....- DELETE -- feature id found";
                     QgsFeatureIds removeIds;
                     removeIds.insert(foundFeature.id());
                     lMemoryBased->dataProvider()->deleteFeatures(removeIds);
                     lMemoryBased->deleteFeature(foundFeature.id());
                     lMemoryBased->commitChanges();
                     lMemoryBased->updateFields();
                     mapCanvas->refresh();

                 }
              }
        }

        if(timerCounter > 9)
            timerCounter=0;
        else
            featureIdCnt++;
       // timerCounter=0;
    }

    qDebug()<<"trackAddUpdateTimerUpdateSLOT....- end";


}

