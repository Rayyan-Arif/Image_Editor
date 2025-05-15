#include <QApplication>
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QFont>
#include <QPixmap>
#include <QFileDialog>
#include <QString>
#include <algorithm>
#include <QTimer>
#include <QTime>
using namespace std;


/* In every function that applies filter to an image, we first convert the image from pixmap to QImage to manipulate pixels and
 then reconvert the image back to QPixmap */

//Class Tint which applie has different filters for an image
class Tint {
public:
    QImage Green(QImage Original) {
        QImage Edited = Original;
        for (int i = 0; i < Edited.width(); i++) {
            for (int j = 0; j < Edited.height(); j++) {
                QColor Color = Edited.pixelColor(i,j);

                Color.setRed(max(0, Color.red()*4/10));
                Color.setGreen(min(255, max(0, (Color.green()*8 / 10) + 30)));
                Color.setBlue(min(255, max(0, Color.blue()*4 / 10)));

                Edited.setPixelColor(i,j, Color);
            }
        }
        return Edited;
    }

    QImage Gray(QImage Original){
        QImage Edited = Original;
        for (int i = 0; i < Edited.width(); i++) {
            for (int j = 0; j < Edited.height(); j++) {
                QColor Color = Edited.pixelColor(i,j);
                float Val = Color.red()*0.299 + Color.green()*0.587 + Color.blue()*0.144;
                Color.setRedF(Val/255.0);
                Color.setGreenF(Val/255.0);
                Color.setBlueF(Val/255.0);
                Edited.setPixelColor(i,j, Color);
            }
        }
        return Edited;
    }
};

//Class ImageEditor inherits QWidget class so that we can set it's components

class ImageEditor : public QWidget{
private:
    QPushButton *Browse , *Blur , *GrayScale , *Sapia , *Invert , *Rotate ,
                *Flip_180 , *Pixel , *GreenTint , *Heat, *Posterize , *Original , *Save , *EdgeDetector;
    QPixmap OriginalImage , ChangedImage;
    QLabel *DisplayChangedImage , *DisplayOriginalImage;
    Tint ImageTint;
    QString ImagePath;
    int ScreenLength, ScreenWidth;
public:
    //This function changes colour of all buttons to default (black)
    void ChangeButtonColours(){
        Blur->setStyleSheet("background-color: black; color: white; font-style: italic;");
        GrayScale->setStyleSheet("background-color: black; color: white; font-style: italic;");
        Sapia->setStyleSheet("background-color: black; color: white; font-style: italic;");
        Invert->setStyleSheet("background-color: black; color: white; font-style: italic;");
        Rotate->setStyleSheet("background-color: black; color: white; font-style: italic;");
        Flip_180->setStyleSheet("background-color: black; color: white; font-style: italic;");
        Pixel->setStyleSheet("background-color: black; color: white; font-style: italic;");
        GreenTint->setStyleSheet("background-color: black; color: white; font-style: italic;");
        Heat->setStyleSheet("background-color: black; color: white; font-style: italic;");
        Posterize->setStyleSheet("background-color: black; color: white; font-style: italic;");
        EdgeDetector->setStyleSheet("background-color: black; color: white; font-style: italic;");
        Original->setStyleSheet("background-color: black; color: white; font-style: italic;");
        Save->setStyleSheet("background-color: black; color: white; font-style: italic;");
        Browse->setStyleSheet("background-color: black; color: white; font-style: italic;");
    }

    void BrowseImage(){     //Function opens the dialog box and we can select images from it
        ChangeButtonColours();
        Browse->setStyleSheet("background-color: blue; color: white; font-style: italic;");

        QString ImagePath = QFileDialog::getOpenFileName(
            this,
            "Open Image",
            "",
            "Image Files (*.png *.jpg *.jpeg);;All Files (*)"
        );
        OriginalImage.load(ImagePath);
        ChangedImage.load(ImagePath);

        OriginalImage = OriginalImage.scaled(500,500,Qt::IgnoreAspectRatio);
        ChangedImage = OriginalImage.scaled(500,500,Qt::IgnoreAspectRatio);
        DisplayOriginalImage->resize(500,500);
        DisplayChangedImage->resize(500,500);

        DisplayOriginalImage->setPixmap(OriginalImage);
        DisplayChangedImage->setPixmap(ChangedImage);
        DisplayChangedImage->move(1000,150);
        DisplayOriginalImage->move(495,150);
    }

    void SaveImage(){       //Function opens the dialog box and we can save files in it
        ChangeButtonColours();
        Save->setStyleSheet("background-color: blue; color: white; font-style: italic;");
        QString ImagePath = QFileDialog::getSaveFileName(
            this,
            "Save File",
            "",
            "PNG Files (*.png);;JPEG Files (*.jpg *.jpeg);;All Files (*)"
        );

        if (!ImagePath.isEmpty()) {
            ChangedImage.save(ImagePath);
        }
    }

    void LoadOriginalImage(){       //It acts like it will reset all the filters of image
        ChangeButtonColours();
        Original->setStyleSheet("background-color: blue; color: white; font-style: italic;");
        ChangedImage = OriginalImage;
        DisplayChangedImage->setPixmap(ChangedImage);
    }

    void BlurredImage() {           //Function to blur the image
        ChangeButtonColours();
        Blur->setStyleSheet("background-color: blue; color: white; font-style: italic;");
        QImage Blurred = ChangedImage.toImage();
        int x = 7;
        for (int i = 0; i < Blurred.width(); i++) {
            for (int j = 0; j < Blurred.height(); j++) {

                int red = 0, green = 0, blue = 0, counter = 0;
                for (int l = -x; l <= x; l++) {
                    for (int m = -x; m <= x; m++) {
                        int px = i+l;
                        int py = j+m;
                        if(px < 0 || px >= Blurred.width() || py < 0 || py >= Blurred.height()){
                            continue;
                        }
                        red += Blurred.pixelColor(px , py).red();
                        counter++;
                        green += Blurred.pixelColor(px , py).green();
                        blue += Blurred.pixelColor(px , py).blue();
                    }
                }
                red /= (counter);
                green /= (counter);
                blue /= (counter);


                QColor color(red, green, blue);
                Blurred.setPixelColor(i,j,color);
            }
        }
        ChangedImage = QPixmap::fromImage(Blurred);
        DisplayChangedImage->setPixmap(ChangedImage);
    }

    void Grayscale() {      //Function to apply grayscale filter to image
        ChangeButtonColours();
        GrayScale->setStyleSheet("background-color: blue; color: white; font-style: italic;");
        ChangedImage = QPixmap::fromImage(ImageTint.Gray(ChangedImage.toImage()));
        DisplayChangedImage->setPixmap(ChangedImage);
    }

    void ApplySapia(){          //Function to apply sapia filter to image
        ChangeButtonColours();
        Sapia->setStyleSheet("background-color: blue; color: white; font-style: italic;");

        QImage Edited = ChangedImage.toImage();
        for (int i = 0; i < Edited.width(); i++) {
            for (int j = 0; j < Edited.height(); j++) {
                QColor Color = Edited.pixelColor(i,j);
                float NewRed = 0.393*Color.red() + 0.769*Color.green() + 0.189*Color.blue();
                float NewGreen = 0.349*Color.red() + 0.686*Color.green() + 0.168    *Color.blue();
                float NewBlue = 0.272*Color.red() + 0.534*Color.green() + 0.131*Color.blue();
                float Min_Red = NewRed < 255 ? NewRed : 255;
                float Min_Green = NewGreen < 255 ? NewGreen : 255;
                float Min_Blue = NewBlue < 255 ? NewBlue : 255;
                Color.setRedF(Min_Red / 255.0);
                Color.setGreenF(Min_Green / 255.0);
                Color.setBlueF(Min_Blue / 255.0);
                Edited.setPixelColor(i,j, Color);
            }
        }
        ChangedImage = QPixmap::fromImage(Edited);
        DisplayChangedImage->setPixmap(ChangedImage);
    }

    void InvertImage(){         //Function to invert the image
        ChangeButtonColours();
        Invert->setStyleSheet("background-color: blue; color: white; font-style: italic;");
        QImage Edited = ChangedImage.toImage();
        for (int i = 0; i < Edited.width(); i++) {
            for (int j = 0; j < Edited.height(); j++) {
                QColor Color = Edited.pixelColor(i,j);
                float NewRed = 255 - Color.red();
                float NewGreen = 255 - Color.green();
                float NewBlue = 255 - Color.blue();
                Color.setRedF(NewRed/255.0);
                Color.setGreenF(NewGreen/255.0);
                Color.setBlueF(NewBlue/255.0);
                Edited.setPixelColor(i,j, Color);
            }
        }
        ChangedImage = QPixmap::fromImage(Edited);
        DisplayChangedImage->setPixmap(ChangedImage);
    }

    void FlipImage(){           //Function to flip the image
        ChangeButtonColours();
        Flip_180->setStyleSheet("background-color: blue; color: white; font-style: italic;");
        QImage Edited = ChangedImage.toImage();
        for (int i = 0; i < Edited.width()/2; i++) {
            for (int j = 0; j < Edited.height(); j++) {
                QColor Color1 = Edited.pixelColor(i,j);
                QColor Color2 = Edited.pixelColor(Edited.width() - i - 1,j);
                Edited.setPixelColor(i , j , Color2);
                Edited.setPixelColor(Edited.width() - i - 1,j, Color1);
            }
        }
        ChangedImage = QPixmap::fromImage(Edited);
        DisplayChangedImage->setPixmap(ChangedImage);
    }

    void Pixelate() {       //Function to apply pixel filter to image
        ChangeButtonColours();
        Pixel->setStyleSheet("background-color: blue; color: white; font-style: italic;");
        int x = 7;
        QImage Edited = ChangedImage.toImage();
        QImage Original = OriginalImage.toImage();
        for (int i = 0; i < Edited.width(); i+=x) {
            for (int j = 0; j < Edited.height(); j+=x) {
                QColor Color = Original.pixelColor(i,j);
                for (int l = i; l < Original.width(); l++) {
                    for (int m = j; m < Edited.height(); m++) {
                        Edited.setPixelColor(l,m, Color);
                        if (m >= j+x) {
                            break;
                        }
                    }
                    if (l >= i+x) {
                        break;
                    }
                }
            }
        }
        ChangedImage = QPixmap::fromImage(Edited);
        DisplayChangedImage->setPixmap(ChangedImage);
    }


    void Rotate90() {           //Function to rotate the image
        ChangeButtonColours();
        Rotate->setStyleSheet("background-color: blue; color: white; font-style: italic;");
        QImage Image = ChangedImage.toImage();
        QImage Edited(Image.height(), Image.width(), QImage::Format_RGB32);
        for (int i = 0; i < Image.width(); i++) {
            for (int j = 0; j < Image.height(); j++) {
                Edited.setPixelColor(j,Image.width()-i, Image.pixelColor(i,j));
            }
        }
        ChangedImage = QPixmap::fromImage(Edited);
        DisplayChangedImage->setPixmap(ChangedImage);
    }

    void PosterizeImage() {         //Function to posterize the image
        ChangeButtonColours();
        Posterize->setStyleSheet("background-color: blue; color: white; font-style: italic;");
        QImage Edited = ChangedImage.toImage();
        QImage Original = OriginalImage.toImage();
        int n = 4;
        for (int i = 0; i < Original.width(); i++) {
            for (int j = 0; j < Original.height(); j++) {
                QColor color = Edited.pixelColor(i,j);
                int red = ((color.red()*(n-1))/256) * 255 / (n-1),
                    green = ((color.green()*(n-1))/256) * 255 / (n-1),
                    blue = ((color.blue()*(n-1))/256) * 255 / (n-1);

                color.setRed(red);
                color.setGreen(green);
                color.setBlue(blue);

                Edited.setPixelColor(i,j, color);
            }
        }
        ChangedImage = QPixmap::fromImage(Edited);
        DisplayChangedImage->setPixmap(ChangedImage);
    }

    void ApplyGreenTint(){          //Function to apply green filter to image
        ChangeButtonColours();
        GreenTint->setStyleSheet("background-color: blue; color: white; font-style: italic;");
        ChangedImage = QPixmap::fromImage(ImageTint.Green(ChangedImage.toImage()));
        DisplayChangedImage->setPixmap(ChangedImage);
    }

    void HeatImage() {              //Function to apply heat filter to image
        ChangeButtonColours();
        Heat->setStyleSheet("background-color: blue; color: white; font-style: italic;");
        QImage Edited = ChangedImage.toImage();
        QImage Original = OriginalImage.toImage();
        int Imin = 255, Imax = 0;
        for (int i = 0; i < Edited.width(); i++) {
            for (int j = 0; j < Edited.height(); j++) {
                QColor color = Original.pixelColor(i,j);
                int I = color.red()*0.299 + color.green()*0.587 + color.blue()*0.114;
                if (Imin > I) {
                    Imin = I;
                }
                if (Imax < I) {
                    Imax = I;
                }
            }
        }
        for (int i = 0; i < Edited.width(); i++) {
            for (int j = 0; j < Edited.height(); j++) {
                QColor color = Edited.pixelColor(i,j);
                float I = color.red()*0.299 + color.green()*0.587 + color.blue()*0.114;
                I = (I-Imin)/float(Imax-Imin);
                if (I <= 0.2) {
                    color.setRgb(0, 0, 255);
                } else if (I <= 0.4) {
                    color.setRgb(0, 128, 255);
                } else if (I <= 0.6) {
                    color.setRgb(173, 255, 47);
                } else if (I <= 0.8) {
                    color.setRgb(255, 165, 0);
                } else {
                    color.setRgb(255, 69, 0);
                }
                Edited.setPixelColor(i,j, color);
            }
        }
        ChangedImage = QPixmap::fromImage(Edited);
        DisplayChangedImage->setPixmap(ChangedImage);
    }

    void EdgeDetection() {                  //Function to apply edge detection filter to image
        ChangeButtonColours();
        EdgeDetector->setStyleSheet("background-color: blue; color: white; font-style: italic;");
        QImage Edited = ChangedImage.toImage();
        QImage Original = OriginalImage.toImage();
        QColor black(0,0,0), white(255,255,255);
        int width = Original.width(), height = Original.height();
        int threshold = 600;
        float intensities[3][3];
        int Kx[3][3] = { {-1,0,1}, {-2,0,2}, {-1,0,1}}, Ky[3][3] = {{-1,-2,-1}, {0,0,0}, {1,2,1}};
        for (int i = 1; i < width-1; i++) {

            Edited.setPixelColor(i,0, black);
            Edited.setPixelColor(i,height-1, black);

            for (int j = 1; j < height-1; j++) {

                int Gx = 0, Gy = 0, G = 0;
                for (int l = -1; l <= 1; l++) {
                    for (int m = -1; m <= 1; m++) {
                        QColor neighbourColor = Original.pixelColor(i+l,j+m);
                        intensities[l+1][m+1] = neighbourColor.red()*0.299 + neighbourColor.green()*0.587 + neighbourColor.blue()*0.114;
                    }
                }
                for (int l = 0; l <= 2; l++) {
                    for (int m = 0; m <= 2; m++) {
                        Gx += intensities[l][m] * Kx[l][m];
                        Gy += intensities[l][m] * Ky[l][m];
                    }
                }

                G = sqrt(Gx*Gx + Gy*Gy);
                G = ((G-min(Gx,Gy))/float(max(Gx,Gy)-min(Gx,Gy))) * 255;

                if (G >= threshold) {
                    Edited.setPixelColor(i,j, white);
                } else {
                    Edited.setPixelColor(i,j, black);
                }

                Edited.setPixelColor(0,j, black);
                Edited.setPixelColor(width-1,j, black);
            }
        }
        ChangedImage = QPixmap::fromImage(Edited);
        DisplayChangedImage->setPixmap(ChangedImage);
    }

    //Function to setup buttons on the screen

    QPushButton* SetButton(string Title , int X , int Y , int L = 400 , int W = 50){
        QPushButton *Button = new QPushButton(QString::fromStdString(Title) , this);
        Button->move(X,Y);
        Button->resize(L,W);
        QFont ButtonFont = Button->font();
        ButtonFont.setPointSize(30);
        Button->setFont(ButtonFont);
        Button->setStyleSheet("background-color: black; color: white; font-style: italic;");
        return Button;
    }

public:
    //Constructor which initializes all the attributes
    ImageEditor(){

        this->move(0,0);
        ScreenLength = 1540;
        ScreenWidth = 790;
        QLabel *BackGroundImage = new QLabel(this);
        QPixmap Image("C:/Users/rayya/OneDrive/Documents/Image_Editor/backgroundImage.png");
        QPixmap ScaledImage = Image.scaled(ScreenLength , ScreenWidth , Qt::IgnoreAspectRatio);

        BackGroundImage->setPixmap(ScaledImage);

        QLabel *AppTitle = new QLabel("PHOTO DUKAAN", this);
        AppTitle->move(590,0);

        QFont TitleFont = AppTitle->font();
        TitleFont.setPointSize(40);
        AppTitle->setFont(TitleFont);
        AppTitle->resize(650,60);
        AppTitle->setStyleSheet("background-color: transparent; color: white; font-weight: bold;");

        Browse = SetButton("BROWSE",10,100);
        Blur = SetButton("1. BLUR", 10,155);
        GrayScale = SetButton("2. GRAYSCALE", 10,210);
        Sapia = SetButton("3. SAPIA", 10,265);
        Invert = SetButton("4. INVERT", 10,320);
        Rotate = SetButton("5. ROTATE", 10,375);
        Flip_180 = SetButton("6. FLIP 180", 10,430);
        Pixel = SetButton("7. PIXEL", 10,485);
        GreenTint = SetButton("8. GREENTINT", 10,540);
        Heat = SetButton("9. HEAT", 10,595);
        Posterize = SetButton("10. POSTERIZE", 10,650);
        EdgeDetector = SetButton("11. EDGEDETECTOR", 10,705);
        Original = SetButton("LOAD ORIGINAL IMAGE", 500, 680,600);
        Save = SetButton("SAVE IMAGE", 1110,680,285);

        DisplayOriginalImage = new QLabel(this);
        DisplayChangedImage = new QLabel(this);

        QTimer *Timer = new QTimer(this);
        int colour = 1;

        QObject::connect(Timer, &QTimer::timeout, [=]() mutable {
            if(colour == 1){
                AppTitle->setStyleSheet("background-color: transparent; color: rgb(0, 128, 128); font-weight: bold;");
            } else if(colour == 2){
                AppTitle->setStyleSheet("background-color: transparent; color: rgb(255, 127, 80); font-weight: bold;");
            } else if(colour == 3){
                AppTitle->setStyleSheet("background-color: transparent; color: rgb(255, 215, 0); font-weight: bold;");
            }
            colour++;
            if(colour > 3) colour = 1;
        });

        Timer->start(700);

        connect(GrayScale , &QPushButton::clicked , this , &ImageEditor::Grayscale);
        connect(Sapia , &QPushButton::clicked , this , &ImageEditor::ApplySapia);
        connect(Invert , &QPushButton::clicked , this , &ImageEditor::InvertImage);
        connect(Blur , &QPushButton::clicked , this , &ImageEditor::BlurredImage);
        connect(Flip_180 , &QPushButton::clicked , this , &ImageEditor::FlipImage);
        connect(Rotate , &QPushButton::clicked , this , &ImageEditor::Rotate90);
        connect(Pixel , &QPushButton::clicked , this , &ImageEditor::Pixelate);
        connect(Posterize , &QPushButton::clicked , this , &ImageEditor::PosterizeImage);
        connect(GreenTint , &QPushButton::clicked , this , &ImageEditor::ApplyGreenTint);
        connect(Heat , &QPushButton::clicked , this , &ImageEditor::HeatImage);
        connect(Original , &QPushButton::clicked , this , &ImageEditor::LoadOriginalImage);
        connect(Browse , &QPushButton::clicked , this , &ImageEditor::BrowseImage);
        connect(Save , &QPushButton::clicked , this , &ImageEditor::SaveImage);
        connect(EdgeDetector , &QPushButton::clicked , this , &ImageEditor::EdgeDetection);

        this->resize(ScreenLength,ScreenWidth);
    }

    ~ImageEditor(){
        delete Browse ;
        delete Blur ;
        delete GrayScale ;
        delete Sapia ;
        delete Invert ;
        delete Rotate ;
        delete Flip_180 ;
        delete Pixel ;
        delete GreenTint ;
        delete Heat;
        delete Posterize ;
        delete Original ;
        delete Save ;
        delete EdgeDetector;
        delete DisplayChangedImage ;
        delete DisplayOriginalImage;
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);           //QApplication handles the main event loop
    ImageEditor editor;
    editor.show();                      //This function shows all the UI
    return app.exec();                     //This starts the functioning of the UI
}
