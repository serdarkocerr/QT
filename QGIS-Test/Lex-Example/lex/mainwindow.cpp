#include "mainwindow.h"
#include <qgis_core.h>
#include <qgis_gui.h>
#include <qgis.h>
#include "ui_mainwindow.h"
#include <QString>
#include <qgsrasterlayer.h>
#include <QDebug>
#include <qgspallabeling.h>
#include <qgsvectorlayerlabeling.h>
#include <qgspropertycollection.h>
#include <qgssymbol.h>
#include <iostream>
#include <qgsrulebasedlabeling.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setMinimumSize(800,400);
    mapCanvas = new QgsMapCanvas();
    mapCanvas->setCanvasColor(Qt::white);

    playout  = new QVBoxLayout;
    playout->setContentsMargins(0,0,0,0);
    playout->addWidget(mapCanvas);

    centralWidget()->setLayout(playout);
//    loadMap();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadMap()
{
    try {
        qDebug()<<"map loading.... begin";
    //    QString currentDir = QDir::currentPath();
    //    QString filename = currentDir + QString("");
        QString currentDirtfw="D:/Qt5.12.12/workspace/Lex-Example/data/NE1_HR_LC_SR_W_DR.tif";
        QgsRasterLayer *lRaster = new QgsRasterLayer(currentDirtfw,"basemap");
        QgsProject::instance()->addMapLayer(lRaster);

        QString currentDirShp ="D:/Qt5.12.12/workspace/Lex-Example/data/ne_10m_populated_places.shp";
        QgsVectorLayer *lVector = new QgsVectorLayer(currentDirShp,"landmarks","ogr");
        QgsProject::instance()->addMapLayer(lVector);

//        /** labels are unreadable so that some label( landmark labels) will be shown due to zooms levels.*/
//        QgsSymbol *symbol = QgsSymbol::defaultSymbol(lVector->geometryType());
//        QgsRuleBasedRenderer *renderer = new QgsRuleBasedRenderer(symbol);
//        arrangeLandMarks(lVector,renderer,symbol);
//        renderer->rootRule()->removeChildAt(0);
//        lVector->setRenderer(renderer);
//       // lVector->triggerRepaint();
//        /** */

//        /** Location Label visible-begin */
//        QgsPalLayerSettings label_settings;
//        label_settings.fieldName="NAME";
//        label_settings.displayAll=true;
//        label_settings.drawLabels=true;
//        label_settings.placement = QgsPalLayerSettings::OverPoint;
//        label_settings.quadOffset = QgsPalLayerSettings::QuadrantBelow;
//        label_settings.yOffset = 1;
//        label_settings.isExpression=true;

//        QgsVectorLayerSimpleLabeling *lab = new QgsVectorLayerSimpleLabeling(label_settings);
//        lVector->setLabeling(lab);
//        lVector->setLabelsEnabled(true);
//        lVector->triggerRepaint();
//        /** Location Label visible-end  */


        QgsPalLayerSettings *label_settings = new QgsPalLayerSettings();
        label_settings->fieldName="NAME";
        label_settings->displayAll=true;
        label_settings->drawLabels=true;
        label_settings->placement = QgsPalLayerSettings::OverPoint;
        label_settings->quadOffset = QgsPalLayerSettings::QuadrantBelow;
        label_settings->yOffset = 1;
        label_settings->isExpression=true;
//        std::string exp = std::string("('CASE WHEN SCALERANK IN (0,1) THEN 18 '") +
//                std::string("'WHEN SCALERANK IN (2, 3, 4) THEN 14 '") +
//                std::string("'WHEN SCALERANK IN (5, 6, 7) THEN 12 '") +
//                std::string("'WHEN SCALERANK IN (8, 9, 10) THEN 10 '") +
//                std::string("'ELSE 9 END')");

//        QgsProperty prop = QgsProperty::fromExpression(QString::fromStdString(exp),true);

//        prop.setActive(true);
//        QgsPropertyCollection propcol;
//        propcol.setProperty(1,prop);
//        label_settings->setDataDefinedProperties(propcol);

        /** create rules*/
      // QgsRuleBasedLabeling::Rule root_rule =  QgsRuleBasedLabeling::Rule();
        QgsRuleBasedLabeling::Rule *root=  new QgsRuleBasedLabeling::Rule( new QgsPalLayerSettings());
        QgsRuleBasedLabeling::Rule *rule1=  new QgsRuleBasedLabeling::Rule( label_settings);
        rule1->setFilterExpression("(SCALERANK >=0) AND (SCALERANK <=1)");
        rule1->setMinimumScale(99999999);//zoom out
        rule1->setMaximumScale(100);//zoom in
        root->appendChild(rule1);

        QgsRuleBasedLabeling::Rule *rule2=  new QgsRuleBasedLabeling::Rule( label_settings);
        rule2->setFilterExpression("(SCALERANK >=2) AND (SCALERANK <=4)");
        rule2->setMinimumScale(10000000);
        rule2->setMaximumScale(0);
        root->appendChild(rule2);

        QgsRuleBasedLabeling::Rule *rule3=  new QgsRuleBasedLabeling::Rule( label_settings);
        rule3->setFilterExpression("(SCALERANK >=5) AND (SCALERANK <=7)");
        rule3->setMinimumScale(5000000);
        rule3->setMaximumScale(0);
        root->appendChild(rule3);

        QgsRuleBasedLabeling::Rule *rule4=  new QgsRuleBasedLabeling::Rule( label_settings);
        rule4->setFilterExpression("(SCALERANK >=8) AND (SCALERANK <=10)");
        rule4->setMinimumScale(2000000);
        rule4->setMaximumScale(0);
        root->appendChild(rule4);

        lVector->setLabelsEnabled(true);

        QgsRuleBasedLabeling *rules = new QgsRuleBasedLabeling(root);

        lVector->setLabeling(rules);

        QList <QgsMapLayer *> layers;
        layers.append(lVector);
        layers.append(lRaster);

        mapCanvas->setLayers(layers);
        mapCanvas->setVisible(true);
        mapCanvas->setExtent(QgsRectangle(-127.7,24.4,-79.3,49.1));
        mapCanvas->refresh();

        qDebug()<<"map loading.... end";
    }  catch ( std::exception &e) {
            qDebug(e.what());
    }

}

// for QGIS before QGIS 3
void MainWindow::arrangeLandMarks(QgsMapLayer *mapLayer,QgsRuleBasedRenderer *renderer,QgsSymbol *symbol)
{
    QgsVectorLayer* layer = static_cast<QgsVectorLayer*>(mapLayer);
    qDebug("arrangeLandMarks -begin");
    if(layer){
        qDebug("layer is  QgsVectorLayer");
//        QgsSymbol *symbol = QgsSymbol::defaultSymbol(layer->geometryType());
//        QgsRuleBasedRenderer *renderer = new QgsRuleBasedRenderer(symbol);

        QgsRuleBasedRenderer::Rule *root_rule = renderer->rootRule();
        QgsRuleBasedRenderer::Rule *default_rule = root_rule->children().at(0);


        QgsRuleBasedRenderer::Rule *rule1 = default_rule->clone();
        QString filter1 = QString::fromStdString("(SCALERANK >=0) AND (SCALERANK <=1)");
//        rule1->setFilterExpression('(SCALERANK >=0) AND (SCALERANK <=1)');
        rule1->setFilterExpression(filter1);
        rule1->setMinimumScale(0);
        rule1->setMaximumScale(99999999);
        root_rule->appendChild(rule1);


        QgsRuleBasedRenderer::Rule *rule2 = default_rule->clone();
        QString filter2 = QString::fromStdString("(SCALERANK >=2) AND (SCALERANK <=4)");
//        rule2->setFilterExpression("(SCALERANK  >=2) AND (SCALERANK <=4)");
        rule2->setFilterExpression(filter2);
        rule2->setMinimumScale(0);
        rule2->setMaximumScale(10000000);
        root_rule->appendChild(rule2);

        QgsRuleBasedRenderer::Rule *rule3 = default_rule->clone();
//        rule3->setFilterExpression("(SCALERANK >=5) AND (SCALERANK <=7)");
        QString filter3 = QString::fromStdString("(SCALERANK >=5) AND (SCALERANK <=7)");
        rule3->setFilterExpression(filter3);
        rule3->setMinimumScale(0);
        rule3->setMaximumScale(5000000);
        root_rule->appendChild(rule3);

        QgsRuleBasedRenderer::Rule *rule4 = default_rule->clone();
//        rule4->setFilterExpression("(SCALERANK >=7) AND (SCALERANK <=10)");
        QString filter4 = QString::fromStdString("(SCALERANK >=7) AND (SCALERANK <=10)");
        rule4->setFilterExpression(filter4);

        rule4->setMinimumScale(0);
        rule4->setMaximumScale(2000000);
        root_rule->appendChild(rule4);

//        root_rule->removeChildAt(0);
//        layer->setRenderer(renderer);
        //layer->triggerRepaint();

        qDebug("arrangeLandMarks -end");

    }else{
        //TODO
        qDebug("TODO");
    }


}

