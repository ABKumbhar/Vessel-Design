#include <iostream>
#include<cmath>
using namespace std;
/*Following includes some of the functions to calculate some variables during design*/

//This function interpolates between two values
float interpolation(float y1,float y2,float x1,float x2,float x0)
{float y0;
y0=(x0-x2)*(y2-y1)/(x2-x1)+y2;
return y0;

}
//This function calculates C value for torispherical head through interpolation

float Cvalue(float hebyDo,float tbyDo)
{float He[6]={0.15,0.2,0.25,0.3,0.4,0.5};
float t[5]={0.002,0.005,0.01,0.02,0.04};
float C[6][5]={{4.55,2.66,2.15,1.95,1.75},{2.3,1.7,1.45,1.37,1.32},{1.38,1.14,1,1,1},{0.92,0.77,0.77,0.77,0.77},{0.59,0.59,0.59,0.59,0.59},{0.55,0.55,0.55,0.55,0.55}};

int i,j,i1,j1;
for(int i1=0;i1<6;i1++)
    {if(hebyDo<He[i1]) {i=i1-1;break;}
    }
for(int j1=0;j1<5;j1++)
    {if(tbyDo<t[j1]) {j=j1-1;break;}
    }

next:
    cout<<i<<" "<<j<<endl;
    float C1,C2,C11,C22,C33,C44,Cfinal;
    C1 = C[i][j];
    C2 = C[i][j+1];
    C11 = C[i+1][j];
    C22 = C[i+1][j+1];
    C33 = interpolation(C1,C2,t[j],t[j+1],tbyDo);
    C44= interpolation(C11,C22,t[j],t[j+1],tbyDo);
    Cfinal= interpolation(C33,C44,He[i],He[i+1],hebyDo);
return Cfinal;

}

//This gives standard values from BCB page 269 Table B1


float Std(float t)
{
float a[]={5,5.5,6,7,8,9,10,11,12,14,16,18,20,22,25,28,32,36,40,45,50,56,63,71,80};
for(int i=0;i<25;i++)
{if((t*1000)<=a[i]){return a[i]/1000;break;}
}
}
//This calculates thickness of cone

float ConeThick(float alpha,float J,float f,float Do,float Di,float tcalShell,float Ca,float Pd)
{float t1,t2,thead=0,tknuckle=0,Z=0,Dk=0,L=0;
    if(alpha==20){Z=1;}
    else if(alpha==30){Z=1.35;}
    else if(alpha==45){Z=2.05;}
    else Z=60;
    cout<<"The value of Z from BCB 49 is "<<Z<<endl;
    tknuckle=Pd*Do*Z/(2*f*J*1000000);
    t2=tknuckle;
    tknuckle=tknuckle+Ca;
    tknuckle=Std(tknuckle);
    L=0.5*pow((Do*tknuckle/cos(alpha*3.141592/180)),0.5);
    Dk=Do-2*tcalShell-2*L*sin(alpha*3.141592/180);
    thead=Pd*Dk/(2*f*J*1000000-Pd)/cos(alpha*3.141592/180);
    t1=thead;
    thead=thead+Ca;
    thead=Std(thead);
if(abs(t1-t2)>=0.001) cout<<"The conical head have thickness at junction equal to "<<tknuckle*1000<<" mm and away from junction as "<<thead*1000<<" mm\n";
else cout<<"Since difference between thickness at junction and away from junction is less than 1mm; the conical head have thickness"<<((thead>tknuckle)?thead:tknuckle)*1000<<" mm\n";
return thead;
}
//This calculates design temperature
void TempDesign(float Tw)
{float itr=0,Td=250;
cout<<"Which of the following case is applicable for your pressure vessel?\n";
cout<<"1)Unheated parts where working temperature will be same as design temperature\n 2)Heated by means of steam, hot water and similar heating media\n ";
cout<<"3)Vessel is shielded and have internal or external heating by fire, flue-gas or electricity\n 4)Vessel is unshielded and have internal or external heating by fire, flue-gas or electricity\n";
cin>>itr;
if(itr==1)
{Td=Tw;}
else if(itr==2){Td=Tw+10;}
else if(itr==3){Td=Tw+20;}
else if(itr==4){Td=Tw+50;}
else Td=250;
if(Td>=250){cout<<"The design temperature is "<<Td<<" degree celcius"<<endl;}
else cout<<"The design temperature is "<<250<<" degree celcius"<<endl;
}
float WeldJoint(int a)
{float itr1=0;
if(a==1) return 1;
else if(a==2)
{cout<<"Which of the following construction you have chosen?\n 1)Single Plate Joint\n 2)Double welded joint with backing strip\n";
cin>>itr1;
if(itr1==1){return 0.8;}
else return 0.85;}
else
{cout<<"Please Specify weld joint efficiency value\n";
cin>>itr1;
return itr1;}
}
float Calculate_he(float Do,float Ro,float ro)
{float hE,ho,h1,h2;
ho= Ro-(pow((Ro-Do/2)*(Ro+Do/2-2*ro),0.5));
h1=pow(Do,2)/4/Ro;
h2=pow((Do*ro/2),0.5);
return ho<(h1<h2?h1:h2)?ho:(h1<h2?h1:h2);
}
//This calculates design pressure
float Pdesign(float Pw,float head)
{
float Pd;
if(head>=(0.05*Pw)) Pd = Pw+head;
else Pd = 1.05*Pw;
return Pd;
}
//This calculates thickness of shell
float Shell(float Pd,float f,float J,float Di,float Do,float Ca)
{float tshell;
if(Do!=0) {tshell=(Pd*Do)/(2*f*1000000*J+Pd);}
else {tshell=Pd*Di/(2*f*1000000*J-Pd);}
return tshell;
}

int main()
{ float De=0,itr1=0,tcalShell=0,tcone=0,alpha=30,Ca=1,f=1,C=0,hE=0,titr=0,head=0,Pw=0,Tw=0,Pd=0,trshell=0,itr=0,J=1,Di=0,Do=0,Ro=0,Ri=0,ri=0,ro=0,tnew=0,tstdShell=0;
cout<<"Following programme calculates shell thickness, head thickness and nozzle requirement for pressure vessel according to  the IS-2825 1969 code for unfired pressure vessel\n-------------------------------------------------------\n";
SHELL_THiCKNESS :
    cout<<"Enter working pressure in Pascal"<<endl;
    cin>>Pw;
    cout<<"Is hydrostatic pressure present? If yes, type 1 or else type 0"<<endl;
    cin>>itr;
    if(itr) {cout<<"Enter head pressure in Pascal"<<endl; cin>>head;}
    Pd=Pdesign(Pw,head);
    cout<<"Hence design pressure is "<<Pd<<"Pa\n";
    cout<<"Enter working temperature in degree celcius"<<endl;
    cin>> Tw;
    TempDesign(Tw);
    cout<<"Specify Class of the vessel. Type 1,2,3 for respective class\n 1)Class I\n 2)Class II\n 3) Class III \n";
    cin>>itr;
    J=WeldJoint(itr);
    cout<<"Enter the allowable stress value from BCB page 261-268 in MPa. For example, if f is 1.98*10^(8)Pa then type value 198 in MPa \n";
    cin>>f;
    cout<<"Enter corrosion allowance in mm\n";
    cin>>Ca;
    Ca=(Ca/1000);
    cout<<"Which diameter of vessel you want to enter?\n 1)Inner\n 2)Outer\n";
    cin>>itr;
    if(itr==1){cout<<"Enter inner diameter in meter\n";cin>>Di;}
    else {cout<<"Enter outer diameter in meter\n";cin>>Do;}
    trshell = Shell(Pd,f,J,Di,Do,Ca);

    if((trshell*1000)<=30) tstdShell=Std(trshell+Ca);
    else tstdShell=Std(trshell);
    if(Do!=0){Di=Do-2*(tstdShell-Ca);}
    else {Do=Di+2*(tstdShell-Ca);}
    bool check=((tstdShell-Ca/Do)<0.25);
    if(check) cout<<"Since t/Do="<<((tstdShell-Ca)/Do)<<". The given vessel is thin walled with standard shell thickness equal to "<<tstdShell*1000<<"mm\n";
    tcalShell=tstdShell-Ca;
HEAD_DESIGN:
    cout<<"Do you want to design head(1/0)\n";
    cin>>itr;
    if(itr){cout<<"Which of the following head you want to design\n 1)Standard Dished Head(including hemispherical and ellipsoidal)\n 2)Conical head\n 3)Flat head\n ";
    cin>>itr;
    if(itr==1) goto Tori;
    else if(itr==2) goto Cone;
    else if(itr==3)  goto Flat;

    }
    else goto Nozzle;

cout<<"--------------------"<<"Following is the programme to calculate torispherical head"<<"------------------------"<<endl;
Tori:
    titr=tstdShell-Ca;
    tnew= tstdShell-Ca;
Step2:
    Do=Di+titr;
Step3:
    cout<<"Enter knuckle inside radius in meter\n";
    cin>>ri;
    cout<<"Enter inner crown radius in meter\n";
    cin>>Ri;
Step4:
    titr=tnew-Ca;
    Ro=Ri+ titr;
    ro=ri+titr;
    hE=Calculate_he(Do,Ro,ro);
    C=Cvalue((hE/Do),(titr/Do));
    //C value from table 4.1A) BCB for hE/Do and t/Do through interpolation
    tnew= Pd*Do*C/(2*f*J*1000000);
    tnew=tnew+Ca;
    tnew=1.06*tnew;
    tnew=Std(tnew);
    if(abs((tnew-Ca)-(titr))<=1) {cout<<"The value of head thichness is"<<tnew*1000<< "mm"<<endl;}
    else {titr=(titr+tnew-Ca)/2;goto Step4;}
    goto Nozzle;
Cone:
    cout<<"Enter half of apex angle (alpha) in degrees(20,30,45,60)\n";
    cin>>alpha;
    ConeThick(alpha,J,f,Do,Di,tcalShell,Ca,Pd);
    goto Nozzle;
Flat:
    float Cf;
    cout<<"Please specify effective diameter(De) from page 45 of BCB in meter\n";
    cin>>De;
    cout<<"Please enter value of C from page 45 of BCB in meter\n";
    cin>>Cf;
    float tflat;
    tflat=Cf*De*pow((Pd/f),0.5)*0.001;

    if(tflat<=0.03){tflat=tflat+Ca;}

    tflat=Std(tflat);
    cout<<"The given flat head have standard thickness equal to "<<tflat*1000<<" mm\n";
    goto Nozzle;
Nozzle:
    cout<<"Do you want to check ring pad requirement for your nozzle(1/0)\n";
    cin>>itr;
    if(itr){cout<<"Where do you want to design nozzle?\n 1)Shell \n2)Head\n";
    cin>>itr1;
    cout<<"Enter the standard thickness as calculated from previous part in mm (shell or head depending upon position of nozzle)\n";
    cin>>titr;
    titr=titr/1000;
    float tp=0,Adash=0,Ar=0,A=0,tr=0,trdash=0,H1=0,H2=0,Ai=0,Ao=0,As=0,d=0,ts=0,tn=0,H1d=0,H2d=0;
    if(itr1==2){cout<<"Enter theoretical thickness of head in mm\n";cin>>tr;tr=tr/1000;}
    else tr=Shell(Pd,f,1,Di,Do,Ca);

    cout<<"Enter inside diameter of nozzle in mm \n";
    cin>>d;
    d=d/1000;
    trdash = Shell(Pd,f,1,d,0,Ca);
    tn=trdash+Ca;
    tn=Std(tn);
    ts=titr;

    As=(d+2*Ca)*(ts-tr-Ca);
    cout<<"Enter the value of outside protrusion in mm\n";
    cin>>H1d;H1d=H1d/1000;
    cout<<"Enter the value of inside protrusion in mm\n";
    cin>>H2d;H2d=H2d/1000;
    H1=H1d<(pow((d+2*Ca)*(tn-Ca),0.5)?H1d:(pow((d+2*Ca)*(tn-Ca),0.5)));
    H2=H2d<(pow((d+2*Ca)*(tn-2*Ca),0.5)?H1d:(pow((d+2*Ca)*(tn-Ca),0.5)));
    Ao=2*H1*(tn-trdash-Ca);
    Ai=2*H2*(tn-2*Ca);
    Adash=As+Ai+Ao;
    A=(d+2*Ca)*tr;
    if(Adash>A){cout<<"Since area to be compensated is smaller ; we do not require ring pad\n";}
    else
        {Ar=A-Adash;
    tp=Ar/(2*(d+2*Ca)-(d+2*Ca+2*trdash));
    cout<<"In this nozzle, ring pad will be required with thickness equal to "<<Std(tp)*1000<<" mm\n";
        }

    }
Opening:
    cout<<"Do you want to check for uncompensated openings made on to the shell?\n";
    cin>>itr;
    if(itr){

      float d0,value,phii;
      float shellarray[]={0,0.25,0.5,0.75,1,1.5,2,2.5,3,3.5,4,4.5,5,5.5,6,6.5,7,7.5,8};
      float phi[] = {1,0.9,0.785,0.7,0.645,0.545,0.465,0.39,0.34,0.285,0.245,0.215,0.18,0.155,0.13,0.115,0.09,0.08,0.075};

        {cout<<"Enter the value of opening diameter\n";
        cin>>d0;
        value = d0/(pow((Do*(tstdShell-Ca)),0.5));
        for(int i=0;i<19;i++)
            {if(value<=shellarray[i])
            {phii=interpolation(phi[i],phi[i-1],shellarray[i],shellarray[i-1],value);break;
            }

            }
       float t;
       t=Shell(Pd,f,phii,Di,Do,Ca);cout<<t;
       if(t<=(tstdShell-Ca)) cout<<"As per German code given opening can be made in the shell without any nozzle or compensation\n";
        else cout<<"As per German code given opening cannot be made in the shell \n";
        }
    }

}



