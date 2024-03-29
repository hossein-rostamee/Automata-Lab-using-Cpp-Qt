#include "graphviewdfa.h"
#include<QMouseEvent>
#include <QInputDialog>
#include <QMessageBox>
#include <QPixmap>
#include "turingwindow.h"
#include <QPrinter>
#include <QPrintDialog>

TuringWindow::TuringWindow(QMainWindow *parent) :
    QMainWindow(parent),parent(parent),
    ui(new Ui::TuringWindow)
{
    ui->setupUi(this);
    move( 400,200);
    setMaximumSize( size() );
    setMinimumSize( size() );
    setFocus( Qt::MouseFocusReason );
    QColor color(57, 189, 243);
    turing = new QA::Turing;
}
TuringWindow::~TuringWindow()
{
    delete ui;
}

void TuringWindow::setStepByStep(){
    ui->stepByStepView->setNodeListView( turing->getStepByStepNode() );
    ui->stepByStepView->setHeadStepByStep( turing->getHeadStepByStep() );
    ui->stepByStepView->setHeadTapeStepByStep( turing->getHeadTapeStepByStep() );
}

void TuringWindow::on_pushButton_clicked()
{
    Edge::setDigitWeigth( 25 );
    Node::color = QColor(82, 191, 85);

    GraphViewDfa::nodeButtonActivated    = true;

    GraphViewDfa::edgeButtonActivated    = false;
    GraphViewDfa::selectButtonActivated  = false;
    GraphViewDfa::removeButtonActivated  = false;

    QPixmap pixmap(":/img/img/iconfinder_add_134224.png");
    QCursor cursor( pixmap.scaled( 20 ,20 ) );
    setCursor( cursor );

    ui->pushButton->setStyleSheet( "border-radius : 10px;"
                                   " background: #6EF772;");
    ui->pushButton_2->setStyleSheet( " border-radius : 10px; "
                                     " background: #9EA8AD; ");
    ui->pushButton_3->setStyleSheet( " border-radius : 10px; "
                                     " background: #9EA8AD; ");
    ui->pushButton_6->setStyleSheet( " border-radius : 10px; "
                                     " background: #9EA8AD; ");
}

void TuringWindow::on_pushButton_2_clicked()
{
    GraphViewDfa::edgeButtonActivated    = true;

    GraphViewDfa::nodeButtonActivated    = false;
    GraphViewDfa::selectButtonActivated  = false;
    GraphViewDfa::removeButtonActivated  = false;

    QCursor cursor;
    cursor.setShape( Qt::DragLinkCursor );
    setCursor( cursor );

    ui->pushButton_2->setStyleSheet( "border-radius : 10px;"
                                   " background: #6EF772;");
    ui->pushButton->setStyleSheet( " border-radius : 10px; "
                                     " background: #9EA8AD; ");
    ui->pushButton_3->setStyleSheet( " border-radius : 10px; "
                                     " background: #9EA8AD; ");
    ui->pushButton_6->setStyleSheet( " border-radius : 10px; "
                                     " background: #9EA8AD; ");
}

void TuringWindow::on_pushButton_3_clicked()
{
    GraphViewDfa::selectButtonActivated  = true;

    GraphViewDfa::edgeButtonActivated    = false;
    GraphViewDfa::nodeButtonActivated    = false;
    GraphViewDfa::removeButtonActivated  = false;

    setCursor( QCursor() );

    ui->pushButton_3->setStyleSheet( "border-radius : 10px;"
                                   " background: #6EF772;");
    ui->pushButton->setStyleSheet( " border-radius : 10px; "
                                     " background: #9EA8AD; ");
    ui->pushButton_6->setStyleSheet( " border-radius : 10px; "
                                     " background: #9EA8AD; ");
    ui->pushButton_2->setStyleSheet( " border-radius : 10px; "
                                     " background: #9EA8AD; ");
}

void TuringWindow::on_pushButton_6_clicked()
{
    GraphViewDfa::removeButtonActivated  = true;

    GraphViewDfa::edgeButtonActivated    = false;
    GraphViewDfa::nodeButtonActivated    = false;
    GraphViewDfa::selectButtonActivated  = false;

    QPixmap pixmap(":/img/img/f6f1a77645c7.png");
    QCursor cursor( pixmap.scaled( 30 ,30 ) );
    setCursor( cursor );

    ui->pushButton_6->setStyleSheet( "border-radius : 10px;"
                                   " background: #6EF772;");
    ui->pushButton->setStyleSheet( " border-radius : 10px; "
                                     " background: #9EA8AD; ");
    ui->pushButton_3->setStyleSheet( " border-radius : 10px; "
                                     " background: #9EA8AD; ");
    ui->pushButton_2->setStyleSheet( " border-radius : 10px; "
                                     " background: #9EA8AD; ");
}

void TuringWindow::on_actionRun_Turing_triggered()
{
    disableAllButtons();

    QString inputString = QInputDialog::getText( this, "ورودی ماشین", "مقدار ورودی را وارد کنید .." );
    while( inputString == "" ){
        inputString = QInputDialog::getText( this, "ورودی ماشین", "مقدار ورودی را دوباره وارد کنید .." );
    }

    Node* initialNode = ui->graphicsView->getInitialNode();
    if ( initialNode ) turing = new QA::Turing( initialNode ,inputString.toStdString() );
    else {
        QMessageBox::critical( this ,"نقص فنی" ,"رأس اولیه مشخص نشده است!!");
        return;
    }

    if ( turing->Run() ) {
        showSuccesfullMessage();
        setStepByStep();
    }
    else showFailedMessage();
}

void TuringWindow::on_horizontalSlider_actionTriggered(int action)
{
    int actionValue = ui->horizontalSlider->value();
    static int prevAction;
    int diffrence = actionValue - prevAction;
    if ( diffrence > 0 )   {
        scaleView(qreal(1.05));
        ui->statusbar->showMessage( "Zoomed In!..." );
    }
    else if ( diffrence < 0 ){
        scaleView(1 / qreal(1.05));
        ui->statusbar->showMessage( "Zoomed Out!..." );
    }
    prevAction = actionValue;
}

void TuringWindow::scaleView(qreal scaleFactor)
{
    qreal factor = ui->graphicsView->transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.5 || factor > 10)
        return;

    ui->graphicsView->scale(scaleFactor, scaleFactor);
}

void TuringWindow::showSuccesfullMessage(){
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setWindowModality(Qt::ApplicationModal);
    msgBox->setInformativeText("ورودی شما قبول شد!");
    msgBox->setIconPixmap( QPixmap( ":/img/img/Tatice-Cristal-Intense-Ok.ico" ).scaled(100,100) );
    msgBox->setStyleSheet( msgBox->styleSheet() );
    msgBox->exec();
}

void TuringWindow::showFailedMessage(){
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setWindowModality(Qt::ApplicationModal);
    msgBox->setInformativeText("ورودی شما رد شد!");
    msgBox->setIconPixmap( QPixmap( ":/img/img/cancel-512.png" ).scaled(100,100) );
    msgBox->exec();
}

void TuringWindow::disableAllButtons(){
    GraphViewDfa::nodeButtonActivated    = false;
    GraphViewDfa::edgeButtonActivated    = false;
    GraphViewDfa::selectButtonActivated  = false;
    GraphViewDfa::removeButtonActivated  = false;

    ui->pushButton_6->setStyleSheet( "border-radius : 10px;"
                                   " background: #9EA8AD;");
    ui->pushButton->setStyleSheet( " border-radius : 10px; "
                                     " background: #9EA8AD; ");
    ui->pushButton_3->setStyleSheet( " border-radius : 10px; "
                                     " background: #9EA8AD; ");
    ui->pushButton_2->setStyleSheet( " border-radius : 10px; "
                                     " background: #9EA8AD; ");

    ui->runButton->setStyleSheet   ( " border-radius : 10px; "
                                     " background: #9EA8AD; ");
    ui->nextButton->setStyleSheet  ( " border-radius : 10px; "
                                     " background: #9EA8AD; ");
    ui->deleteButton->setStyleSheet( " border-radius : 10px; "
                                     " background: #9EA8AD; ");
    setCursor( QCursor() );
}

void TuringWindow::on_runButton_clicked()
{
    disableAllButtons();

    if ( turing->getStepByStepNode().length() == 0 ){
        QMessageBox msgBox(this);
        msgBox.setText( "هیچ ماشین تورینگ موفقی موجود نیست..." );
        msgBox.exec();
    }
    else {
        ui->runButton->setStyleSheet("background: #6EF772;"
                                     "border-top-left-radius:18px;"
                                     "border-top-right-radius: 10px;"
                                     "border-bottom-right-radius: 18px;"
                                     "border-bottom-left-radius:5px;");

        ui->stepByStepView->setEdgeListView( turing->getStepByStepEdge() );
        ui->stepByStepView->setNodeListView( turing->getStepByStepNode() );
        ui->stepByStepView->play();
    }
}

void TuringWindow::on_nextButton_clicked()
{
    disableAllButtons();

    if ( ui->stepByStepView->getPlaySituation() ) {
        ui->nextButton->setStyleSheet("background: #6EF772;"
                                     "border-top-left-radius:18px;"
                                     "border-top-right-radius: 10px;"
                                     "border-bottom-right-radius: 18px;"
                                     "border-bottom-left-radius:5px;");

        ui->stepByStepView->next();
    }
    else QMessageBox::warning( this, "نقص فنی :(" , "ابتدا دکمه اجرا را بزنید...");
}

void TuringWindow::on_deleteButton_clicked()
{
    disableAllButtons();
    ui->deleteButton->setStyleSheet("background: #6EF772;"
                                 "border-top-left-radius:18px;"
                                 "border-top-right-radius: 10px;"
                                 "border-bottom-right-radius: 18px;"
                                 "border-bottom-left-radius:5px;");
    ui->stepByStepView->emptyScene();
}

void TuringWindow::on_backToMainWindow_clicked()
{
    this->close();

    Node::isInitialNodeHasBeenChoosen = false;
    Node::counter = 0;
    disableAllButtons();

    this->parent->show();
}

void TuringWindow::on_actionMultiple_Run_triggered()
{
    Node* initialNode = ui->graphicsView->getInitialNode();
    if ( !initialNode ) {
        QMessageBox::critical( this ,"نقص فنی" ,"رأس اولیه مشخص نشده است!!");
        return;
    }

    int n = QInputDialog::getInt( this, "گرفتن تعداد ورودی ها" ,"تعداد ورودی ها خود را وارد کنید..." );
    QString* string = new QString[ n ];
    for ( int i=0 ; i<n ; i++){
        std::string String = "ورودی ماشین شماره" + std::to_string( i + 1 ) + " را وارد کنید" ;
        QString inputString = QInputDialog::getText( this, "ورودی ماشین" , QString::fromStdString( String ) );
        while( inputString == "" ){
            inputString = QInputDialog::getText( this, "ورودی ماشین", "مقدار ورودی را دوباره وارد کنید .." );
        }
        string[ i ] = inputString;
    }
    bool* results = new bool[ n ];
    QList<QA::Turing*> TuringMachine;
    for ( int i=0 ; i<n ; i++ ){
        QA::Turing* Turing = nullptr;
        Turing = new QA::Turing( initialNode ,string [ i ].toStdString() );
        TuringMachine << Turing;
    }
    for ( int i=0 ; i<n ; i++ ){
        bool result = TuringMachine[ i ]->Run();
        results[ i ] = result;
    }

    class Dialog : public QDialog
    {
    public :
        Dialog( QWidget* parent ,bool* results ,QString* string ,int n ):QDialog( parent ){
            this->results = results;
            this->string = string;
            this->n = n;
            setGeometry( 400 , 400 ,400 , 30 * n + 2 );
        }
    protected:
        void paintEvent( QPaintEvent* e ){
            QPainter painter( this );
            painter.setFont( QFont("Times New Roman" ,18 ,20 ) );
            QRectF* stringRect = new QRectF[ n ];
            for ( int i=0 ; i<n ; i++ ){
                stringRect[ i ] = QRectF( 0 , 30 * i , 200 , 30 );
                painter.drawRect( stringRect[i] );
                painter.drawText( stringRect[i] ,Qt::AlignCenter ,string[i] );
            }
            QRectF* resultRect = new QRectF[ n ];
            for ( int i=0 ; i<n ; i++ ){
                resultRect[ i ] = QRectF( 200 , 30 * i , 199 , 30 );
                painter.drawRect( resultRect[ i ] );
                painter.drawText( resultRect[i] ,Qt::AlignCenter ,results[i] ? tr("true") : tr("false") );
            }


        }
    private:
        bool* results = nullptr;
        QString* string = nullptr;
        int n = 0;
    };

    Dialog dialog( this ,results ,string ,n );
    dialog.exec();
}

void TuringWindow::on_actionMake_PDF_triggered()
{
    QPrinter printer;
    QPrintDialog dialog( &printer ,this );
    printer.setPrinterName( "برنامه سازی پیشرفته ۹۷۲" );
    if ( dialog.exec() == QDialog::Rejected ) return;
    printer.setPageOrientation( QPageLayout::Landscape );
    QPainter painter(&printer);
    painter.setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setSceneRender( &painter );
}

void TuringWindow::on_actionSave_triggered()
{
    QImage img(1024,768,QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&img);
    ui->graphicsView->setSceneRender( &painter );
    painter.end();
    img.save( "Turing.png" );
    QMessageBox::information( this, "ذخیره ", "عکس در پوشه اپلیکیشن ذخیره شد ...");
}
