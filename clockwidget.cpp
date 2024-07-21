#include "clockwidget.h"
#include <QPainter>
#include <QTimerEvent>
#include <QPoint>
#include <cmath>
#include <QTime>
#include <QImage>

ClockWidget::ClockWidget(QWidget *parent) :
    QWidget(parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    int size = 1300; //boyutunda çiziyoruz
    int size2= 350;
    setFixedSize(size, size2);
    timerId = startTimer(1000);
    setTime();
}

ClockWidget::~ClockWidget()
{
   killTimer(timerId);
   timerId = 0;
}
void ClockWidget::setTime()
{
   QTime now = QTime::currentTime();
   //analog saat
   hourHand = (now.hour()%12)*5;
   minuteHand = now.minute();
   secondsHand = now.second();
   //digital saat
   digithour = QString("%1").arg(now.hour(), 2, 10, QChar('0'));
   digitminute = QString("%1").arg(now.minute(), 2, 10, QChar('0'));
   digitsec = QString("%1").arg(now.second(), 2, 10, QChar('0'));
   digit = digithour + ":" + digitminute + ":" + digitsec;
}

void ClockWidget::paintEvent(QPaintEvent *event)
{

   Q_UNUSED(event);

   //arkaplana renk vermek için oluşturulan çizim
   QPainter painter2(this);
   int rectWidth = width();
   int rectHeight = height();
   int rectX = 0;
   int rectY = 0;

   painter2.setRenderHint(QPainter::Antialiasing, true);
   QColor arkaplan(10, 64, 64);
   painter2.setBrush(QBrush(arkaplan, Qt::SolidPattern));
   painter2.drawRect(rectX, rectY, rectWidth, rectHeight);

   QPixmap image(":/images/Abramak Logo ENG[7741].png");
   int imageWidth = width()/4; //  genişlik değeri
   int imageHeight = height()/5; // yükseklik değeri

   QPixmap scaledImage = image.scaled(imageWidth, imageHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
   // Pencerenin ortasında hizalama yapmak için gerekli konumları hesapla
   int imageX = imageWidth/5;
   int imageY = (height()/2)-imageHeight/2;

   painter2.drawPixmap(imageX, imageY, scaledImage);


   //saat yuvarlağının çizimi
   QPainter painter(this);
   int cap = height();
   center.setX(width()/2);
   center.setY(height()/2);

   int radius = qMin(cap/(2.2), cap/(2.2));
   painter.setRenderHint(QPainter::Antialiasing, true);
   QColor metal(84, 105, 79);
   painter.setBrush(QBrush(metal, Qt::SolidPattern));
   painter.drawEllipse(center, radius, radius);

   //digital saat
   painter.setPen(Qt::white); // Metin rengini ve yazı tipini ayarlayalım
   QFont font("OCR A Extended", 20);
   painter.setFont(font);
   //çerçeve çizimi
   QFontMetrics fontMetrics(font);
   int textWidth = fontMetrics.horizontalAdvance(digit);
   //konumlandırma
   int x = center.x()-textWidth/2; // X koordinatı
   int y = center.y()+cap/4; // Y koordinatı
   painter.drawText(x, y, digit);

   // Saat işaretlerini çizme
   for (int i = 0; i < 12; ++i) {
       double angle = toRadian(i * 5);
       double outerX = center.x() + 0.9 * radius * cos(angle);
       double outerY = center.y() + 0.9 * radius * sin(angle);
       double innerX = center.x() + 0.8 * radius * cos(angle);
       double innerY = center.y() + 0.8 * radius * sin(angle);

       QColor asker1(10, 64, 64);
       painter.setPen(QPen(asker1, 2));
       painter.drawLine(outerX, outerY, innerX, innerY);
   }
   // Dakika işaretlerini çizme
   for (int i = 0; i < 60; ++i) {
       if (i % 5 == 0) // Zaten saat işaretlerini çizdik, dakika işaretlerini sadece 5 aralıklarla çizeceğiz
           continue;

       double angle = toRadian(i);
       double outerX = center.x() + 0.9 * radius * cos(angle);
       double outerY = center.y() + 0.9 * radius * sin(angle);
       double innerX = center.x() + 0.85 * radius * cos(angle);
       double innerY = center.y() + 0.85 * radius * sin(angle);

       painter.setPen(QPen(Qt::white, 1));
       painter.drawLine(outerX, outerY, innerX, innerY);
   }

   // Saat, dakika ve saniye işaretlerini(akrep,yelkovan,saniye kolu)çizme
   double hourX = center.x() + 0.3 * radius * cos(toRadian(hourHand));
   double hourY = center.y() + 0.3 * radius * sin(toRadian(hourHand));
   double minuteX = center.x() + 0.5 * radius * cos(toRadian(minuteHand));
   double minuteY = center.y() + 0.5 * radius * sin(toRadian(minuteHand));
   double secondX = center.x() + 0.7 * radius * cos(toRadian(secondsHand));
   double secondY = center.y() + 0.7 * radius * sin(toRadian(secondsHand));


   // Dakika işareti(yelkovan)
   painter.setPen(QPen(Qt::white, 3));
   painter.setRenderHint(QPainter::Antialiasing);
   painter.drawLine(center.x(), center.y(), minuteX, minuteY);

   // Saat işareti(akrep)
   QColor asker2(10, 64, 64);
   painter.setPen(QPen(asker2, 4));
   painter.drawLine(center.x(), center.y(), hourX, hourY);

   // Saniye işareti(saniye kolu)
   QColor asker3(120,134,107);
   painter.setPen(QPen(asker3, 1));
   painter.setRenderHint(QPainter::Antialiasing);
   painter.drawLine(center.x(), center.y(), secondX, secondY);

   // Saatin merkezini gösteren yuvarlak
   QColor specialcolor(255, 215, 0);
   QPen pen(specialcolor,5);
   pen.setCapStyle(Qt::RoundCap); // Kalem uç stilini yuvarlak (RoundCap) yapar
   painter.setPen(pen);
   painter.drawPoint(center);

   // Saniye işareti ucundaki yuvarlak
   painter.setPen(QPen(Qt::white, 2));
   painter.drawPoint(secondX, secondY);


}
void ClockWidget::timerEvent(QTimerEvent *te)
{
   if (te->timerId() == timerId) {
      secondsHand = (secondsHand + 1) % 60;

      if(secondsHand == 0) {

         minuteHand = (minuteHand + 1) % 60;

         if (minuteHand == 0) {

            hourHand = (hourHand + 5) % 60;
         }
      }
     setTime();
     repaint();
   }
   else {
      QWidget::timerEvent(te);
   }
}
