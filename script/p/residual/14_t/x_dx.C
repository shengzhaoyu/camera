{
//gPad->SetFixedAspectRatio(1);
//gStyle->SetOptTitle(kFALSE);
gStyle->SetOptStat(0);
//define variables
float xpix;
float ypix;
float magx; float magy;
float view = (TMath::Pi()/2.118);
TVector3 pmt;
double theta = 0;

std::vector<TVector3>pmtList;	

ifstream myReadFile;
myReadFile.open("../../paras14.txt");
float para[14];
int ip=0;
if (myReadFile.is_open()) {
while (!myReadFile.eof()) {

   myReadFile >> para[ip];
   //cout<<para[ip]<<endl;
   ip++;
}
}
myReadFile.close();

double mx1 = para[0];
TVector3 xcp(para[1],para[2],para[3]);
float angle = para[4];  
float rx =  para[5];
float ry = para[6];
double mx0 = para[7];double mx2 = para[8];double mx3 = para[9];
double my1 = para[10];
double my0 = para[11];double my2 = para[12];double my3 = para[13];


float d;
double deltay;


ifstream myReadFile2;
myReadFile2.open("../../cpmts.txt");
float xc[136], yc[136], zc[136];
int ip2=0;
TVector3 temp;
if (myReadFile2.is_open()) {
while (ip2<136) {

   myReadFile2 >> xc[ip2] >> yc[ip2] >> zc[ip2];
   //cout<<xc[ip2]<<" "<<yc[ip2]<<" "<<zc[ip2]<<endl;
   temp.SetX(xc[ip2]); temp.SetY(yc[ip2]); temp.SetZ(zc[ip2]);
   pmtList.push_back(temp);
   ip2++;
}
}myReadFile2.close();

ifstream myReadFile3;
myReadFile3.open("../../pos.txt");
std::string linep;
getline(myReadFile3,linep);
float number[136], xc2[136], yc2[136];
int ip3=0;
if (myReadFile3.is_open()) {
while (ip3<136.) {

   myReadFile3 >> number[ip3] >> xc2[ip3] >> yc2[ip3];
   //cout<<xc2[ip3]<<endl;

   ip3++;
}
}
myReadFile3.close();

//form the image            

TVector3 north(sin(angle),cos(angle),0);
TVector3 ko(xcp.Unit());
TVector3 io(north-(ko * north) * ko); io=io.Unit();
TVector3 jo(ko.Cross(io));

TVector3 k(ko + tan(rx)*io + tan(ry)*jo); k = k.Unit();
TVector3 i(north-(k * north) * k); i=i.Unit();
TVector3 j(k.Cross(i));

TH2F *picture = new TH2F ("HPicture","calculated",4310,-2155,2155,60,-30,30);
picture->SetMarkerStyle(8);
picture->SetMarkerSize(0.2);

int q = 0;
double tr = 0;
while (q<136) {
  pmt = pmtList[q];
  TVector3 kp(-pmt.Unit());
  TVector3 ip3(north-(kp * north) * kp); ip3=ip3.Unit();
  TVector3 jp(kp.Cross(ip3));
  TVector3 xprime((pmt-xcp) * i,(pmt-xcp) * j, (pmt-xcp) * k);
  theta = abs(atan(sqrt((TMath::Power(xprime.X(),2))+(TMath::Power(xprime.Y(),2)))/(xprime.Z())));
  d = (sqrt((TMath::Power(xprime.X(),2))+(TMath::Power(xprime.Y(),2))));
  //mag = m0 + m1*theta/d + m2*TMath::Power(theta,2)/d + m3*TMath::Power(theta,3)/d;


  magx = mx0 + mx1*theta/d + mx2*TMath::Power(theta,2)/d + mx3*TMath::Power(theta,3)/d;
  magy = my0 + my1*theta/d + my2*TMath::Power(theta,2)/d + my3*TMath::Power(theta,3)/d;

  xpix = xprime.X()*magx;
  ypix = xprime.Y()*magy;

  deltay = xpix - xc2[q];
   tr = tr + deltay;
   cout<<tr<<endl;
   //cout<<xc2[q]<<" "<<xpix<<" "<<magx<<endl;
   picture->Fill(xpix,deltay);
   q++;
}

picture->Draw();

c1->SetCanvasSize(4310*0.2,2868*0.2);
picture->GetXaxis()->SetTitle("calculated x pixal position");
picture->GetYaxis()->SetTitle("delta x(number of pixels)");

picture->SetTitle("x position vs delta x");
//TF1*f = new TH1("func","x",-2000.,2000);
//f->Draw("same");








c1->SaveAs("x_dx14.pdf");
}





