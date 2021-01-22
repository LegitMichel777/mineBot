#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <unistd.h>
using namespace std;
typedef long long ll;
struct coord {
    ll x,y;
};
inline bool douEq(double x,double y) {
    return (abs(x-y)<=0.00000001);
}
bool sizeCmp(vector<coord> const &a,vector<coord> const &b) {
    return a.size()<b.size();
}
int main() {
    while (true) {
        ifstream in("/Users/legitmichel777/Desktop/sweep/botCom.txt");
        string seeifL;
        getline(in,seeifL);
        if (seeifL=="LOOK") {
            string rtKey;
            getline(in,rtKey);
            in.close();
            ofstream rt("/Users/legitmichel777/Desktop/sweep/botCom.txt");
            rt.close();
            rt.open("/Users/legitmichel777/Desktop/sweep/mineCom.txt");
            rt<<"ACK"<<endl<<rtKey;
            rt.close();
            bool gotHandshake=false;
            ll tmOutLim=30;
            ll tmOutCnt=0;
            while (true) {
                in.open("/Users/legitmichel777/Desktop/sweep/botCom.txt");
                getline(in,seeifL);
                if (seeifL=="HANDSHAKE") {
                    string verfK;
                    getline(in,verfK);
                    in.close();
                    ofstream rt("/Users/legitmichel777/Desktop/sweep/botCom.txt");
                    rt.close();
                    if (rtKey==verfK) {
                        gotHandshake=true;
                        break;
                    }
                }
                in.close();
                usleep(1e4);
                tmOutCnt++;
                if (tmOutCnt>tmOutLim) break;
            }
            if (gotHandshake) break;
            cout<<"Attempted connection timed out"<<endl;
        }
        usleep(1e5);
    }
    cout<<"Successfully connected"<<endl;
    vector<vector<ll>>gamem;
    vector<vector<ll>>marke;
    ll minesL,szx,szy;
    szx=szy=-1;
    bool virgin=true;
    while (true) {
        ifstream in("/Users/legitmichel777/Desktop/sweep/botCom.txt");
        string seeifL;
        getline(in,seeifL);
        in.close();
        if (seeifL=="MAP") {
            ofstream out("/Users/legitmichel777/Desktop/sweep/botCom.txt");
            out.close();
            in.open("/Users/legitmichel777/Desktop/sweep/botComBig.txt");
            string fElement;
            //parse
            while (!in.eof()) {
                getline(in,fElement);
                if (fElement=="MINESLEFT") {
                    string tmp;
                    getline(in,tmp);
                    minesL=atoll(tmp.c_str());
                } else if (fElement=="MAPSIZEX") {
                    string tmp;
                    getline(in,tmp);
                    szx=atoll(tmp.c_str());
                } else if (fElement=="MAPSIZEY") {
                    string tmp;
                    getline(in,tmp);
                    szy=atoll(tmp.c_str());
                } else if (fElement=="MAP") {
                    string mpRd;
                    getline(in,mpRd);
                    if (mpRd=="EMPTY") {
                        if (szx==-1||szy==-1) {
                            cout<<"Non-initialized board!"<<endl;
                        }
                        gamem.clear();
                        marke.clear();
                        for (ll i=0;i<szx;i++) {
                            vector<ll>insT;
                            vector<ll>markT;
                            for (ll j=0;j<szy;j++) {
                                insT.push_back(-1);
                                markT.push_back(0);
                            }
                            marke.push_back(markT);
                            gamem.push_back(insT);
                        }
                        virgin=true;
                    } else {
                        for (int i=0;i<szy;i++) {
                            gamem[0][i]=atoll(mpRd.substr(0,mpRd.find(' ')).c_str());
                            mpRd.erase(0,mpRd.find(' ')+1);
                        }
                        for (int i=1;i<szx;i++) {
                            getline(in,mpRd);
                            for (int j=0;j<szy;j++) {
                                gamem[i][j]=atoll(mpRd.substr(0,mpRd.find(' ')).c_str());
                                mpRd.erase(0,mpRd.find(' ')+1);
                            }
                        }
                    }
                }
            }
            if (virgin) {
                int myCM=rand()%4;
                ofstream maMove("/Users/legitmichel777/Desktop/sweep/mineComBig.txt");
                maMove<<"OPEN"<<endl;
                switch(myCM) {
                    case 0:maMove<<0<<endl<<0;break;
                    case 1:maMove<<0<<endl<<szy-1;break;
                    case 2:maMove<<szx-1<<endl<<0;break;
                    case 3:maMove<<szx-1<<endl<<szy-1;break;
                    default:cout<<"ERROR!"<<endl;
                }
                maMove.close();
            } else {
                ll dir[8][2]={{0,1},{0,-1},{1,0},{-1,0},{1,-1},{1,1},{-1,-1},{-1,1}};
                vector<coord>nxtOpen;
                vector<coord>nowMark;
                
                for (ll i=0;i<szx;i++) {
                    for (ll j=0;j<szy;j++) {
                        if (gamem[i][j]>0) {
                            ll cum=0;
                            for (ll k=0;k<8;k++) {
                                ll nx=i+dir[k][0];
                                ll ny=j+dir[k][1];
                                if (nx>=0&&nx<szx&&ny>=0&&ny<szy) {
                                    cum+=(gamem[nx][ny]==-1);
                                }
                            }
                            if (cum==gamem[i][j]) {
                                for (ll k=0;k<8;k++) {
                                    ll nx=i+dir[k][0];
                                    ll ny=j+dir[k][1];
                                    if (nx>=0&&nx<szx&&ny>=0&&ny<szy) {
                                        if (marke[nx][ny]==0&&gamem[nx][ny]==-1) {
                                            nowMark.push_back((coord){nx,ny});
                                            marke[nx][ny]=1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                for (ll i=0;i<szx;i++) {
                    for (ll j=0;j<szy;j++) {
                        if (gamem[i][j]>0) {
                            ll cum=0;
                            for (ll k=0;k<8;k++) {
                                ll nx=i+dir[k][0];
                                ll ny=j+dir[k][1];
                                if (nx>=0&&nx<szx&&ny>=0&&ny<szy) {
                                    cum+=(marke[nx][ny]==1);
                                }
                            }
                            if (cum==gamem[i][j]) {
                                for (ll k=0;k<8;k++) {
                                    ll nx=i+dir[k][0];
                                    ll ny=j+dir[k][1];
                                    if (nx>=0&&nx<szx&&ny>=0&&ny<szy) {
                                        if (marke[nx][ny]!=1&&gamem[nx][ny]==-1) {
                                            nxtOpen.push_back((coord){nx,ny});
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if (minesL==0) {
                    for (ll i=0;i<szx;i++) {
                        for (ll j=0;j<szy;j++) {
                            if (gamem[i][j]==-1&&marke[i][j]!=1) {
                                nxtOpen.push_back((coord){i,j});
                            }
                        }
                    }
                }
                if (nxtOpen.size()==0) { //remove nowMark for speed
                    //LOGICCC
                    //dfs
                    bool vst[szx][szy];
                    bool shoExp[szx][szy];
                    double probs[szx][szy];
                    short enumVirgin[szx][szy];
                    for (ll i=0;i<szx;i++) {
                        for (ll j=0;j<szy;j++) {
                            vst[i][j]=0;
                            shoExp[i][j]=0;
                            probs[i][j]=0;
                            enumVirgin[i][j]=1;
                        }
                    }
                    for (ll i=0;i<szx;i++) {
                        for (ll j=0;j<szy;j++) {
                            if (gamem[i][j]==-1&&marke[i][j]==0) {
                                bool isExpBlk=false;
                                for (ll k=0;k<8;k++) {
                                    ll nx=i+dir[k][0];
                                    ll ny=j+dir[k][1];
                                    if (nx>=0&&nx<szx&&ny>=0&&ny<szy) {
                                        if (gamem[nx][ny]>0) {
                                            isExpBlk=1;
                                            break;
                                        }
                                    }
                                }
                                shoExp[i][j]=isExpBlk;
                            }
                        }
                    }
                    vector<vector<coord>>mstrGrp;
                    ll groups=0;
                    for (ll i=0;i<szx;i++) {
                        for (ll j=0;j<szy;j++) {
                            if (vst[i][j]==0&&shoExp[i][j]) {
                                groups++;
                                vector<coord>disGrp;
                                vector<coord>checkNums;
                                stack<coord>dfs;
                                dfs.push((coord){i,j});
                                vst[i][j]=1;
                                disGrp.push_back((coord){i,j});
                                while (!dfs.empty()) {
                                    coord nxt=dfs.top();
                                    dfs.pop();
                                    for (ll k=0;k<8;k++) {
                                        ll nx=nxt.x+dir[k][0];
                                        ll ny=nxt.y+dir[k][1];
                                        if (nx>=0&&nx<szx&&ny>=0&&ny<szy) {
                                            if (((gamem[nx][ny]>0&&gamem[nxt.x][nxt.y]==-1)||(gamem[nx][ny]==-1&&marke[nx][ny]==0&&shoExp[nx][ny]))&&vst[nx][ny]==0) {
                                                vst[nx][ny]=1;
                                                dfs.push((coord){nx,ny});
                                                if (gamem[nx][ny]==-1) disGrp.push_back((coord){nx,ny});
                                            }
                                        }
                                    }
                                }
                                mstrGrp.push_back(disGrp);
                            }
                        }
                    }
                    if (mstrGrp.size()) sort(mstrGrp.begin(),mstrGrp.end(),sizeCmp);
                    ll blksLeft[szx][szy];
                    for (ll i=0;i<szx;i++) {
                        for (ll j=0;j<szy;j++) {
                            if (gamem[i][j]>0) {
                                ll blk=0;
                                for (ll k=0;k<8;k++) {
                                    ll nx=i+dir[k][0];
                                    ll ny=j+dir[k][1];
                                    if (nx>=0&&nx<szx&&ny>=0&&ny<szy) {
                                        if (gamem[nx][ny]==-1&&marke[nx][ny]!=1) blk++;
                                    }
                                }
                                blksLeft[i][j]=blk;
                            }
                        }
                    }
                    for (ll i=0;i<mstrGrp.size();i++) {
                        vector<coord>disGrp=mstrGrp[i];
                        ll totPerms=0;
                        //dfs args:Current object ID,set current object to
                        //dfs steps:
                        ///First verify configuration.
                        //write to main array marke and make sure to erase everything
                        struct dfsCont {
                            ll objID,objSet;
                        };
                        stack<dfsCont>enu;
                        enu.push((dfsCont){0,0});
                        enu.push((dfsCont){0,2});
                        ll lstObjID=0;
                        while (!enu.empty()) {
                            //reset gamem and enumVirgin
                            dfsCont cur=enu.top();
                            ll disX=disGrp[cur.objID].x;
                            ll disY=disGrp[cur.objID].y;
                            enu.pop();
                            if (lstObjID>=cur.objID) {
                                //reset
                                for (ll k=cur.objID;k<=lstObjID;k++) {
                                    marke[disGrp[k].x][disGrp[k].y]=0;
                                    enumVirgin[disGrp[k].x][disGrp[k].y]=1;
                                    //                                            cout<<"SETBACK "<<disGrp[k].x<<' '<<disGrp[k].y<<endl;
                                }
                            }
                            lstObjID=cur.objID;
                            //verify config
                            //trust that the previous config is valid
                            //TODO:add truMine property to avoid scanning
                            //if everything around a object has already been enumerated/touched, then see if it equals it's value
                            bool valConfig=true;
                            //DO ADDING INVALID CHECK
                            if (cur.objSet==2) {
                                for (ll k=0;k<8;k++) {
                                    ll nx=disX+dir[k][0];
                                    ll ny=disY+dir[k][1];
                                    if (nx>=0&&nx<szx&&ny>=0&&ny<szy) {
                                        if (gamem[nx][ny]>0) {
                                            ll cum=1;
                                            for (ll k2=0;k2<8;k2++) {
                                                ll nx2=nx+dir[k2][0];
                                                ll ny2=ny+dir[k2][1];
                                                if (nx2>=0&&nx2<szx&&ny2>=0&&ny2<szy) {
                                                    cum+=(marke[nx2][ny2]>0);
                                                }
                                            }
                                            //                                                    cout<<"PROCESSING FOR "<<disX<<' '<<disY<<", VALIDATING "<<nx<<' '<<ny<<" CUM "<<cum<<' '<<gamem[nx][ny]<<endl;
                                            if (cum>gamem[nx][ny]) {
                                                valConfig=false;
                                                break;
                                            }
                                        }
                                    }
                                }
                                if (mstrGrp.size()==1) {
                                    ll minesLc=minesL;
                                    if (minesLc<=cur.objID) {
                                        for (ll k=0;k<cur.objID;k++) {
                                            minesLc+=(marke[disGrp[k].x][disGrp[k].y]==2);
                                        }
                                    }
                                }
                                if (!valConfig) continue;
                            }
                            //DO VALIDATE CHECK
                            
                            for (ll k=0;k<8;k++) {
                                ll nx=disX+dir[k][0];
                                ll ny=disY+dir[k][1];
                                if (nx>=0&&nx<szx&&ny>=0&&ny<szy) {
                                    if (gamem[nx][ny]>0) {
                                        ll validBlocks=1;
                                        ll blocksLeft=blksLeft[nx][ny]; //blocksLeft:Blocks that can be counted
                                        for (ll k2=0;k2<8;k2++) {
                                            ll nx2=nx+dir[k2][0];
                                            ll ny2=ny+dir[k2][1];
                                            if (nx2>=0&&nx2<szx&&ny2>=0&&ny2<szy) {
                                                if (!enumVirgin[nx2][ny2]) validBlocks++;
                                                //a valid block is something that ain't already marked
                                            }
                                        }
                                        if (validBlocks==blocksLeft) {
                                            ll curMarked=(cur.objSet==2);
                                            for (ll k2=0;k2<8;k2++) {
                                                ll nx2=nx+dir[k2][0];
                                                ll ny2=ny+dir[k2][1];
                                                if (nx2>=0&&nx2<szx&&ny2>=0&&ny2<szy) {
                                                    if (marke[nx2][ny2]>0) curMarked++; //a block that's hidden in the game board,
                                                }
                                            }
                                            if (gamem[nx][ny]!=curMarked) {
                                                valConfig=false;
                                            }
                                            
                                            //v a l i d a t e
                                        }
                                    }
                                }
                            }
                            if (!valConfig) continue;
                            marke[disX][disY]=cur.objSet;
                            enumVirgin[disX][disY]=false;
                            if (cur.objID==disGrp.size()-1) {
                                totPerms++;
                                for (ll k=0;k<disGrp.size();k++) probs[disGrp[k].x][disGrp[k].y]+=(marke[disGrp[k].x][disGrp[k].y]==2);
                                /*
                                 usleep(1e5);
                                 ofstream maMove("/Users/legitmichel777/Desktop/sweep/mineComBig.txt");
                                 for (ll bl1=0;bl1<szx;bl1++) {
                                 for (ll bl2=0;bl2<szy;bl2++) {
                                 if (marke[bl1][bl2]==2) maMove<<"MARK"<<endl<<bl1<<endl<<bl2<<endl;
                                 }
                                 }
                                 maMove.close();
                                 ofstream conf("/Users/legitmichel777/Desktop/sweep/mineCom.txt");
                                 conf<<"MOVE";
                                 conf.close();
                                 cout<<"I GOT A CONFIG!"<<endl;
                                 maMove.open("/Users/legitmichel777/Desktop/sweep/mineComBig.txt");
                                 for (ll bl1=0;bl1<szx;bl1++) {
                                 for (ll bl2=0;bl2<szy;bl2++) {
                                 if (marke[bl1][bl2]==2) maMove<<"UNMARK"<<endl<<bl1<<endl<<bl2<<endl;
                                 }
                                 }
                                 maMove.close();
                                 conf.open("/Users/legitmichel777/Desktop/sweep/mineCom.txt");
                                 conf<<"MOVE";
                                 conf.close();
                                 */
                            } else {
                                //push new configs in
                                enu.push((dfsCont){cur.objID+1,0});
                                enu.push((dfsCont){cur.objID+1,2});
                            }
                            if (totPerms>100000) {
                                cout<<"BREAKING:PROCESS HAS GONE EXPONENTIAL. OBJECTS TO BE ENUMERATED:"<<disGrp.size()<<endl;
                                break;
                            }
                        }
                        for (ll k=0;k<=lstObjID;k++) {
                            marke[disGrp[k].x][disGrp[k].y]=0;
                            enumVirgin[disGrp[k].x][disGrp[k].y]=1;
                        }
                        //reset marke and enumVirgin
                        for (ll k=0;k<disGrp.size();k++) {
                            probs[disGrp[k].x][disGrp[k].y]/=totPerms;
                            if (douEq(probs[disGrp[k].x][disGrp[k].y],1.0)) {
                                nowMark.push_back((coord){disGrp[k].x,disGrp[k].y});
                                marke[disGrp[k].x][disGrp[k].y]=1;
                            } else if (douEq(probs[disGrp[k].x][disGrp[k].y],0.0)) {
                                nxtOpen.push_back((coord){disGrp[k].x,disGrp[k].y});
                            }
                        }
                        if (nxtOpen.size()) break;
                    }
                    if (mstrGrp.size()==0) {
                        if (gamem[0][0]==-1) {
                            nxtOpen.push_back((coord){0,0});
                        } else if (gamem[0][szy-1]==-1) {
                            nxtOpen.push_back((coord){0,szy-1});
                        } else if (gamem[szx-1][0]==-1) {
                            nxtOpen.push_back((coord){szx-1,0});
                        } else if (gamem[szx-1][szy-1]==-1) {
                            nxtOpen.push_back((coord){szx-szy-1});
                        } else {
                            bool iGotIt=false;
                            for (ll i=0;i<szy;i++) {
                                if (gamem[0][i]==-1) {
                                    nxtOpen.push_back((coord){0,i});
                                    iGotIt=true;
                                    break;
                                } else if (gamem[szx-1][i]==-1) {
                                    nxtOpen.push_back((coord){szx-1,i});
                                    iGotIt=true;
                                    break;
                                }
                            }
                            if (!iGotIt) {
                                for (ll i=0;i<szx;i++) {
                                    if (gamem[i][0]==-1) {
                                        nxtOpen.push_back((coord){i,0});
                                        iGotIt=true;
                                        break;
                                    } else if (gamem[i][szy-1]==-1) {
                                        nxtOpen.push_back((coord){szy-1,i});
                                        iGotIt=true;
                                        break;
                                    }
                                }
                            }
                            if (!iGotIt) {
                                for (ll i=0;i<szx;i++) {
                                    for (ll j=0;j<szy;j++) {
                                        if (gamem[i][j]==-1) {
                                            nxtOpen.push_back((coord){i,j});
                                            iGotIt=true;
                                            break;
                                        }
                                    }
                                    if (iGotIt) break;
                                }
                            }
                        }
                    }
                    double opMin=1.0;
                    coord mxC;
                    if (nxtOpen.empty()) {
                        for (ll i=0;i<szx;i++) {
                            for (ll j=0;j<szy;j++) {
                                if (!douEq(probs[i][j],0)&&opMin>probs[i][j]) {
                                    opMin=probs[i][j];
                                    mxC=(coord){i,j};
                                }
                            }
                        }
                        nxtOpen.push_back(mxC);
                    }
                    //TODO:Do not recalculate if section has not been touched
                }
                ofstream maMove("/Users/legitmichel777/Desktop/sweep/mineComBig.txt");
                for (ll i=0;i<nxtOpen.size();i++) {
                    maMove<<"OPEN"<<endl<<nxtOpen[i].x<<endl<<nxtOpen[i].y<<endl;
                }
                for (ll i=0;i<nowMark.size();i++) {
                    maMove<<"MARK"<<endl<<nowMark[i].x<<endl<<nowMark[i].y<<endl;
                }
                maMove.close();
            }
            ofstream conf("/Users/legitmichel777/Desktop/sweep/mineCom.txt");
            conf<<"MOVE";
            conf.close();
//            cout<<"Bot makes move"<<endl;
            virgin=false;
        } else if (seeifL=="CLOSE") {
            return 0;
        }
        usleep(1e5);
    }
    return 0;
}
