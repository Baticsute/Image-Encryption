#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "BigIntegerLibrary.hh"
#include "BigIntegerAlgorithms.hh"
#include <QString>
#include <QByteArray>
#include <QMessageBox>
#include <vector>
#include <ctime>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QPixmap>
#include <QImage>
#include <QLabel>
#include <QColor>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    srand(time(NULL));
    ui->setupUi(this);
}

QVector<QString> MainWindow::getCodeMask()
{
    return code_mask;
}

void MainWindow::setCodeMask(QVector<QString> a)
{
    code_mask = a;
}

void MainWindow::clearCodeMask()
{
    code_mask.clear();
}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::DisplayImage(QImage &img, QString title)
{
    QLabel *label = new QLabel();
    label->setPixmap(QPixmap::fromImage(img));
    label->setWindowTitle(title);
    label->show();

}

void MainWindow::on_btn_randomkey_2_clicked()
{
    ui->run_randomkey_2->setValue(0);
    ui->tx_plaintext_en_2->setText("");
    ui->tx_encrypted_code_2->setText("");
    code_mask.clear();


    ui->run_randomkey_2->setValue(10);
    int bit,lenght_num;
    BigUnsigned two,one,zero;
    two=2;
    one=1;
    zero=0;
    BigUnsigned p,q,n,phi,e,d,limit;
    ui->run_randomkey_2->setValue(20);
    bit = ui->ln_bit_2->text().toInt();
    limit = pow(two,bit);
    lenght_num = bigUnsignedToString(limit).length();
    ui->run_randomkey_2->setValue(30);
    q = BigInt_Random(lenght_num);
    p = BigInt_Random(lenght_num);
    ui->run_randomkey_2->setValue(40);
    while( (Miller_Rabin(p,30) != 1) || (p>limit) || (p ==zero)){
        p=BigInt_Random(lenght_num);
    }
    while( (Miller_Rabin(q,30) != 1)||(q>limit) || (q==zero)){
        q=BigInt_Random(lenght_num);
    }

    ui->run_randomkey_2->setValue(60);
    string temp;
    temp = bigUnsignedToString(p);
    ui->tx_p_2->setText(QString(temp.data()));
    temp = bigUnsignedToString(q);
    ui->tx_q_2->setText(QString(temp.data()));
    ui->run_randomkey_2->setValue(70);

    n = p*q;
    temp = bigUnsignedToString(n);
    ui->tx_n_2->setText(QString(temp.data()));
    ui->tx_N_encr_2->setText(QString(temp.data()));
    ui->tx_N_decr_2->setText(QString(temp.data()));
    ui->run_randomkey_2->setValue(80);


    phi = (p-1)*(q-1);
    temp = bigUnsignedToString(phi);
    ui->tx_phin_2->setText(QString(temp.data()));

    e=BigInt_Random(lenght_num);
    while((gcd(e,phi)!=one || (e == zero))){
        e=BigInt_Random(lenght_num);
    }
    ui->run_randomkey_2->setValue(90);
    d = Mod_Inverse(e,phi);
    temp = bigUnsignedToString(e);
    ui->tx_E_2->setText(QString(temp.data()));
    ui->tx_E_encr_2->setText(QString(temp.data()));
    temp = bigUnsignedToString(d);
    ui->tx_D_2->setText(QString(temp.data()));
    ui->run_randomkey_2->setValue(100);
    ui->tx_D_decr_2->setText(QString(temp.data()));

   // ui->run_randomkey->setValue(0);

}

void MainWindow::on_bt_browse_2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,"Open File","E:/Data_RSA","*.* All Files;;*.bmp;;*.jpeg;;*.ppm;;*.png,*.jpg");
        if(filename.isEmpty()){
           // QMessageBox::information(this,"Loi !","File khong ton tai !");
            return;
        }
        ui->tx_filename_2->setText(filename);
}

void MainWindow::on_btn_display1_clicked()
{
    QString filename = ui->tx_plaintext_en_2->toPlainText();
    QImage image_in(filename);
    DisplayImage(image_in,QFileInfo(filename).fileName()+" Original");

}

void MainWindow::on_btn_Load_2_clicked()
{
    // send file name to plain text
    QString filename = ui->tx_filename_2->toPlainText();
    ui->tx_plaintext_en_2->setText(filename);
}

void MainWindow::on_btn_encrypted_2_clicked()
{
    if(ui->tx_plaintext_en_2->toPlainText().isEmpty() ||ui->tx_E_encr_2->toPlainText().isEmpty() || ui->tx_N_encr_2->toPlainText().isEmpty()){
        QMessageBox::information(this,"False Step","PlainText Is Null ! Please Type it ..");
    }else
    {
        QString filename = ui->tx_plaintext_en_2->toPlainText();
        QImage image_in(filename); // Ảnh đầu vào
        QImage image_out(image_in.width(),image_in.height(),QImage::Format_ARGB32); // Ảnh sau khi mã hóa
        code_mask.clear(); // chắc rằng code_mask phải rỗng
        BigUnsigned e,n,c_red,c_green,c_blue,m_red,m_green,m_blue;
        BigUnsigned mask_red,mask_green,mask_blue;
        e = stringToBigUnsigned(ui->tx_E_encr_2->toPlainText().toStdString()); // Lấy giá trị e
        n = stringToBigUnsigned(ui->tx_N_encr_2->toPlainText().toStdString());  // Lấy giá trị N
        // Các vong lặp xử lý điểm ảnh
        for (int x=0; x<image_in.width(); x++)
                for (int y=0; y<image_in.height(); y++){

                    QColor rgb = image_in.pixel(x,y); // lấy pixel tại vị trí x, y
                    int red = rgb.red(); // giá trị đỏ
                    int blue = rgb.blue(); // giá trị xanh lam
                    int green = rgb.green(); // giá trị xanh lá
                    m_red = BigUnsigned(red); // chuyển sang kiểu BigInt
                    m_green = BigUnsigned(green);
                    m_blue = BigUnsigned(blue);

                    c_red = a_powx_modn(m_red,e,n); // Mã hóa cho giá trị Đỏ
                    mask_red = BigUnsigned(c_red/255); // Giá trị q được đưa vào Code_mask
                    code_mask.push_back(QString(bigUnsignedToString(mask_red).data()));
                    c_red = c_red % 255; // Đưa về về miền 255

                    c_green = a_powx_modn(m_green,e,n); // Mã hóa cho giá trị Xanh Lá
                    mask_green = BigUnsigned(c_green/255); // Giá trị q được đưa vào Code_mask
                    code_mask.push_back(QString(bigUnsignedToString(mask_green).data()));
                    c_green = c_green % 255; // Đưa về về miền 255

                    c_blue = a_powx_modn(m_blue,e,n);// Mã hóa cho giá trị Xanh Lam
                    mask_blue = BigUnsigned(c_blue/255); // Giá trị q được đưa vào Code_mask
                    code_mask.push_back(QString(bigUnsignedToString(mask_blue).data()));
                    c_blue = c_blue % 255; // Đưa về về miền 255
                    // Gán Pixel ảnh đầu ra
                    image_out.setPixel(x,y,qRgb(c_red.toInt(),c_green.toInt(),c_blue.toInt()));

                }
        QMessageBox::information(this,"Encrypted Completed ","Image was Encrypted !");
        DisplayImage(image_out,QFileInfo(filename).fileName()+" Encrypted");
        // Save file
        QString selectedFilter;

        QString filenamesave = QFileDialog::getSaveFileName(this,"Save Image",QDir::currentPath(),"BMP (*.bmp);;PNG (*.png);;JPEG (*.jpg);; All files (*.*)",&selectedFilter);
                 if(selectedFilter== "BMP (*.bmp)"){
                     image_out.save(filenamesave,"BMP");
                 }
                 if(selectedFilter == "PNG (*.png)"){
                     image_out.save(filenamesave,"PNG");
                 }
                 if(selectedFilter == "JPEG (*.jpg)"){
                     image_out.save(filenamesave,"JPEG");
                 }
                 else
                      image_out.save(filenamesave);
        QFile data("E:/Data_RSA");
        QString filenamesave1 = QFileDialog::getSaveFileName(this,"Save Mask Code",QDir::currentPath(),"RSA Text (*.rsa.txt)");
        data.setFileName(filenamesave1); // Tên File save
        if (!data.open(QIODevice::WriteOnly | QIODevice::Text)) // chỉ ghi dữ liệu từ thiết bị
              return;
        QTextStream ts(&data); // Dữ liệu ghi vào biến data
        ts.setCodec("UTF-8"); // chưa dùng đến
        for(int i =0;i<code_mask.size();i++){ // Các pt của Code_block lần lược ghi lên các dòng tiếp theo
             ts << code_mask[i] << endl;
        }
        QMessageBox::information(this,"Information","Saved RSA File is successful.");

    }
}

void MainWindow::on_bt_decrypted_2_clicked()
{
    if(ui->tx_encrypted_code_2->toPlainText().isEmpty() ||ui->tx_D_decr_2->toPlainText().isEmpty() || ui->tx_N_decr_2->toPlainText().isEmpty() || ui->tx_mask_filename->toPlainText().isEmpty()){
        QMessageBox::information(this,"False Step","PlainText Is Null ! Please Type it ..");
    }else{

        QString filename = ui->tx_encrypted_code_2->toPlainText();
        QString filename_mask = ui->tx_mask_filename->toPlainText();
        QImage image_in(filename); // ảnh đầu vào
        QImage image_out(image_in.width(),image_in.height(),QImage::Format_ARGB32); // ảnh đầu ra
        QFile data(filename_mask); // tên file mask
        if (!data.open(QIODevice::ReadOnly | QIODevice::Text))
               return;
        clearCodeMask(); // làm rỗng code_mask cũ
        QTextStream ts(&data); // đọc file từ data
        QString line,t=""; // dòng lưu tạm
        do{  // đọc tiếp những line kế code_másk
            line = ts.readLine();
            code_mask.push_back(line);
            t=t+line;
        }while(!line.isNull()); // cho đến hết
        data.close(); // đóng file
        // Đã nạp file mask cho code block
        BigUnsigned d,n,c_red,c_green,c_blue,m_red,m_green,m_blue;
        d = stringToBigUnsigned(ui->tx_D_decr_2->toPlainText().toStdString()); // Lấy giá trị D
        n = stringToBigUnsigned(ui->tx_N_decr_2->toPlainText().toStdString()); // Lấy giá trị N
        int index = 0; // biến đọc vào cho từng bước nhảy Pixel
        // index = Red , index +1 = Green , index + 2 = Blue
        // Qua một Pixel tăng lên 3 giá trị
        for (int x=0; x<image_in.width(); x++)
                for (int y=0; y<image_in.height(); y++){

                    QColor rgb = image_in.pixel(x,y); // Lấy giá trị Pixel tại điểm x,y


                    int red = rgb.red();
                    int blue = rgb.blue();
                    int green = rgb.green();
                    BigUnsigned red_mask = stringToBigUnsigned(code_mask[index].toStdString()); // lấy q red
                    BigUnsigned green_mask = stringToBigUnsigned(code_mask[index+1].toStdString()); // lấy q green
                    BigUnsigned blue_mask = stringToBigUnsigned(code_mask[index+2].toStdString()); // lấy q blue

                    c_red = (red_mask*255) + BigUnsigned(red); // Khôi phục giá trị
                    c_green =  (green_mask*255) + BigUnsigned(green);
                    c_blue =  (blue_mask*255) + BigUnsigned(blue);

                    m_red = a_powx_modn(c_red,d,n); // Giải Mã
                    m_green = a_powx_modn(c_green,d,n);
                    m_blue = a_powx_modn(c_blue,d,n);

                    image_out.setPixel(x,y,qRgb(m_red.toInt(),m_green.toInt(),m_blue.toInt()));
                    index += 3; // tăng 3 để đến Pixel ( R,G,B) mới )

                }
        QMessageBox::information(this,"Decrypt Comleted ","Image Was Recoveried !");
        QString selectedFilter;
        QString filenamesave = QFileDialog::getSaveFileName(this,"Save Image",QDir::currentPath(),"BMP (*.bmp);;PNG (*.png);;JPEG (*.jpg);; All files (*.*)",&selectedFilter);
                 if(selectedFilter== "BMP (*.bmp)"){
                     image_out.save(filenamesave,"BMP");
                 }
                 if(selectedFilter == "PNG (*.png)"){
                     image_out.save(filenamesave,"PNG");
                 }
                 if(selectedFilter == "JPEG (*.jpg)"){
                     image_out.save(filenamesave,"JPEG");
                 }
                 else
                      image_out.save(filenamesave);
       DisplayImage(image_out,QFileInfo(filenamesave).fileName()+" Recovery");
    }
}

void MainWindow::on_bt_loadfile_2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,"Open File","E:/Data_RSA","*.* All Files;;*.bmp;;*.jpeg;;*.ppm;;*.png,*.jpg");
        if(filename.isEmpty()){
           // QMessageBox::information(this,"Loi !","File khong ton tai !");
            return;
        }
    ui->tx_encrypted_code_2->setText(filename);

}

void MainWindow::on_btn_display1_2_clicked()
{
    QString filename = ui->tx_encrypted_code_2->toPlainText();
    QImage image_in(filename);
    DisplayImage(image_in,QFileInfo(filename).fileName()+" Encypted");

}



void MainWindow::on_bt_loadmaskfile_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,"Open File","E:/Images","*.rsa.txt");
        if(filename.isEmpty()){
           // QMessageBox::information(this,"Loi !","File khong ton tai !");
            return;
        }
    ui->tx_mask_filename->setText(filename);
}

void MainWindow::on_bt_reset_2_clicked()
{
    ui->tx_plaintext_en_2->setText("");
    ui->tx_D_2->setText("");
    ui->tx_E_2->setText("");
    ui->tx_phin_2->setText("");
    ui->tx_n_2->setText("");
    ui->tx_phin_2->setText("");
    ui->tx_q_2->setText("");
    ui->tx_p_2->setText("");
    ui->tx_filename_2->setText("");
    ui->tx_E_encr_2->setText("");
    ui->tx_D_decr_2->setText("");
    ui->tx_N_encr_2->setText("");
    ui->tx_N_decr_2->setText("");
    ui->tx_encrypted_code_2->setText("");
    ui->tx_mask_filename->setText("");
    ui->run_encrypted_2->setValue(0);
    ui->run_randomkey_2->setValue(0);
}
