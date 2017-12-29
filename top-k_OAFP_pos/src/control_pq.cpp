#include "control_pq.hpp"

using namespace std;

void Control_Pq::Init(const uint32_t kHashsize){
	vhash_.Init(kHashsize);
	lhash_.Init(kHashsize);
  	vector<uint64_t>().swap(fpos);
  	for(int i =0; i < 1000000000; ++i){
    fpos.push_back(0);
  }
}

void Control_Pq::Count(const uint64_t kVar, const uint64_t kLen, const uint64_t kPos){
	if(vhash_.Find(kVar,kPos)){//kVarがある場合はFind内でfreq++
		//cout << " increment" << endl;
		return;
	}

	if(vhash_.IsFull()){ //kVarがハッシュにないかつ、ハッシュに空きがない場合
		if(!priq_.IsLonger(kLen)){ //ボトムより短い場合は何もしない
			//cout << "nop" <<endl;
			return;
		}else{
			//cout << "delete var" <<endl;
			vhash_.Remove(priq_.VarPop()); //ボトム要素を１つpopし、ハッシュから変数を削除
			if(priq_.IsTopEmpty()){
				//cout << "delete list" <<endl;
				lhash_.Remove(priq_.GetLen());  //ボトムの配列に要素がなくなったらハッシュから長さを削除
				priq_.Pop();
			}
		}
	}

	VarData* vd = new VarData;
	vd->Set(kVar,kPos);
	vhash_.Register(vd);  //ハッシュに変数登録
	if(lhash_.IsExist(kLen)){//kLenがハッシュにある場合
		//cout << "push list ";
		lhash_.VarPush(vd, kLen);  //その長さのリストにプッシュ
	}else{//kLenがハッシュにない場合
		//cout << "push pq ";
		VarList* vl = new VarList;
		vl->Set(vd);
		//cout << "v:" << vl.GetVar() << " f:" << vl.GetFreq() <<endl;
		priq_.Push(vl, kLen);  //プライオリティーキューに追加
		lhash_.Register(vl, kLen);
	}
}

void Control_Pq::Print(){
	string cpd_out = "cpd_out.txt";
    ofstream outputfileC(cpd_out,ios::app);
    outputfileC.close();
	while(!priq_.IsEmpty()){
		VarData* vp = priq_.VpPop();
		ofstream outputfileC(cpd_out,ios::app);
		outputfileC << "var:" << vp->GetVar() << " freq:" << vp->GetFreq() << " len:" << priq_.GetLen() << endl;
		delete(vp);
		if(priq_.IsTopEmpty()){
			VarList* vl = priq_.TopVlist();
			delete(vl);
			priq_.Pop();
		}
	}
}

void Control_Pq::PrintDescend(const string& kExperimentFileName){
    std::vector<Ss> reg_;
	while(!priq_.IsEmpty()){
		VarData* vp = priq_.VpPop();
		Ss tmp;
		tmp.var_ = vp->GetVar();
		tmp.freq_ = vp->PosSize();
		tmp.len_ = priq_.GetLen();
		tmp.pos_.push_back(fpos[tmp.var_]);
		for(uint64_t j = 0;j < vp->PosSize();j++){
        	tmp.pos_.push_back(vp->GetPos(j));
    	}
    	reg_.push_back(tmp);
		delete(vp);
		if(priq_.IsTopEmpty()){
			VarList* vl = priq_.TopVlist();
			delete(vl);
			priq_.Pop();
		}
	}
	string experiment_out = kExperimentFileName;
    ofstream outputfileX(experiment_out);
	for(int i = reg_.size()-1; i >=0; --i){
    	outputfileX << reg_[i].len_ << endl;
    	outputfileX << reg_[i].freq_ + 1 << endl;
    	for(uint64_t j = 0;j < reg_[i].pos_.size(); ++j){
      		outputfileX << reg_[i].pos_[j] << endl;
    	}
	}
    outputfileX.close();
}

uint64_t Control_Pq::ByteSize()const{
	return sizeof(Control_Pq) +
			//sizeof(bool) * f_vec.size() +
			vhash_.ByteSize() +
			lhash_.ByteSize() +
			priq_.ByteSize();
}

uint64_t Control_Pq::VSize()const{
	return  vhash_.ByteSize();
}

uint64_t Control_Pq::LSize()const{
	return  lhash_.ByteSize();
}

uint64_t Control_Pq::PqSize()const{
	return  priq_.ByteSize();
}

void Control_Pq::SetPos(const uint64_t kVar, const uint64_t kPos){
	if(fpos[kVar] == 0){
		fpos[kVar] = kPos;
	}
}

/*
uint64_t Control_Pq::FSize()const{
	return  sizeof(bool) * f_vec.size();
}

//f_vec functions
void Control_Pq::AddVar(){
	f_vec.push_back(0);
}

void Control_Pq::FreqVar(const uint64_t kVar){
	f_vec[kVar] = 1;
}

bool Control_Pq::IsFreq(const uint64_t kVar){
	return f_vec[kVar];
}
*/