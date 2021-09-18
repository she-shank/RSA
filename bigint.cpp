#include <bits/stdc++.h>

using namespace std;

#define base 10

class bigint{

    private:
    vector<int> num;
    // void normalize();
    void equalize_length(vector<int> &other_num);
    

    public:
    bigint long_mul(bigint other);
    int isNegative {0};
    bigint();
    bigint(int n);
    bigint(string n);
    bigint(vector<int> n);
    bool operator>(bigint &op);
    bool operator<(bigint &op);
    bigint operator+(bigint op);
    bigint operator-(bigint op);
    bigint operator*(bigint op);
    void print();
};

bigint::bigint(){
}

bigint::bigint(int n){
    div_t divResult;
    if(n<0){
        isNegative = 1;
        n = -n;
    }
    while(n>0){
        divResult = div(n, base);
        num.push_back(divResult.rem);
        n = divResult.quot;
    }
}

bigint::bigint(string n){
    for(auto i = n.rbegin(); i != n.rend(); ++i){
        if(*i>47 && *i<58){
            num.push_back(*i-'0');
        }else if (*i==45){
            isNegative = 1;
        }
    }
}

bigint::bigint(vector<int> n){
    num = n;
}

void bigint::equalize_length(vector<int> &other_num){
    int size_diff = num.size() - other_num.size();
    vector<int> *temp;
    
    if(size_diff < 0){
        temp = &num;
        size_diff = -size_diff;
    }else {
        temp = &other_num;
    }
    
    vector<int> zero_vector(size_diff,0);
    temp->insert(temp->end(), zero_vector.begin(), zero_vector.end());
}

bigint bigint::long_mul(bigint other){
    bigint prod = bigint();
    div_t divResult;
    prod.num.assign(num.size()+other.num.size()+1, 0);
    int carry {0}, i, j, temp;
    vector<int>* l {num.size() > other.num.size() ? &num : &other.num};
    vector<int>* s {num.size() <= other.num.size() ? &num : &other.num};
    
    
    for(i=0; i<s->size(); i++){
        for(j=0; j<l->size(); j++){
            temp =  (s->at(i) * l->at(j)) + carry + prod.num.at(i+j);
            if(temp>=base){
                divResult = div(temp, base);
                temp = divResult.rem;
                carry = divResult.quot;
            }else {
                carry = 0;
            }
            prod.num.at(i+j) = temp;
        }
        if(carry != 0)
            prod.num.at(i+j) += carry;
        carry = 0;
    }
    
    
    if(carry != 0){
        prod.num.at(i+j+1) += carry;
    }
    return prod;
}

bool bigint::operator<(bigint &op){
    int i {0};
    bool res;

    if(isNegative != op.isNegative){
        return isNegative;
    }

    if(num.size() < op.num.size()) {
        res = true;
    }else if(num.size() == op.num.size()) {
        while(i<num.size()){
            if(num.at(i) < op.num.at(i)){
                res = true;
                break;
            }else if(num.at(i) > op.num.at(i)) {
                res = false;
                break;
            }
            i++;
        }
        //case when diff is 0 and bith numbers are equal
        if(i==num.size()){
            res = false;
        }
    }else {
        res = false;
    }

    if(isNegative == op.isNegative && !isNegative){
        res = !res;
    }

    return res;     
}

bool bigint::operator>(bigint &op){
    int i {0};
    bool res;

    if(isNegative != op.isNegative){
        return !isNegative;
    }

    if(num.size() < op.num.size()) {
        res = false;
    }else if(num.size() == op.num.size()) {
        while(i<num.size()){
            if(num.at(i) < op.num.at(i)){
                res = false;
                break;
            }else if(num.at(i) > op.num.at(i)) {
                res = true;
                break;
            }
            i++;
        }
        //case when diff is 0 and bith numbers are equal
        if(i==num.size()){
            res = false;
        }
    }else {
        res = true;
    }

    if(isNegative == op.isNegative && !isNegative){
        res = !res;
    }

    return res; 
}

bigint bigint::operator+(bigint op){
    int carry {0}, i, temp;
    
    
    if(isNegative==1){
        return (op-bigint(num));
    }
    
    bigint sum = bigint();
    equalize_length(op.num);
    for(i=0; i<num.size(); i++){
        temp = num.at(i) + op.num.at(i) + carry;
        if(temp>=base){
            temp = temp%base;
            carry = 1;
        }else {
            carry = 0;
        } 
        sum.num.push_back(temp);
    }

    if(carry == 1){
        sum.num.push_back(1);
    }
    return sum;
}

bigint bigint::operator-(bigint other){
        int leng1 = num.size();
    int leng2 = other.num.size();
    int carry {0}, i {0}, temp;
    // cout<<"here";
    vector<int> *l, *s;
    bigint diff;
    //finding which is larger integer and determining the sign of the diffference
    if(num.size() < other.num.size()) {
        l = &other.num;
        s = &num;
        diff.isNegative = 1;
    }else if(num.size() == other.num.size()) {
        while(i<num.size()){
            // cout<<" here1";
            if(num.at(num.size()-i-1) > other.num.at(num.size()-i-1)){
                // cout<<" here2";
                l = &num;
                s = &other.num;
                break;
            }else if(num.at(num.size()-i-1) < other.num.at(num.size()-i-1)) {
                // cout<<" here3";
                l = &other.num;
                s = &num;
                diff.isNegative = 1;
                break;
            }
                i++;
            
        }
        //case when diff is 0 and bith numbers are equal
        if(i==num.size()){
            l = &num;
            s = &other.num;
        }
    }else {
        l = &num;
        s = &other.num;
    }
    
    // equalize_length(other.num);
    // // vector<int>* l {num.size() > other.num.size() ? &num : &other.num};
    // // vector<int>* s {num.size() <= other.num.size() ? &num : &other.num};
    int length1 = num.size();
    int length2 = other.num.size();
    for(i=0; i<l->size(); i++){
        if(i<s->size()){
            // cout<<" here b temp";
            int t = l->at(i);
            int k = s->at(i);
            temp = l->at(i) - s->at(i) + carry;
            // cout<<" a temp";
            if(temp<0){
                temp = temp+base;
                carry = -1;
            }else {
                carry = 0;
            } 
            diff.num.push_back(temp);
        }else {
            diff.num.push_back(l->at(i)+carry);
        }
    }
    
    return diff;
}

bigint bigint::operator*(bigint op){
    equalize_length(op.num);
    int n = num.size();
    
    bigint prod;
    
    
    
    if(n<3){
        prod = long_mul(op);
        if(isNegative ^ op.isNegative){
        prod.isNegative = 1;
    }
        return prod;
    }
    
    
    
    bigint x2(vector<int>(num.begin(),num.begin()+(n/2)));
    x2.print();
    cout<<endl;
    bigint x1(vector<int>(num.begin()+(n/2),num.end()));
    x1.print();
    cout<<endl;
    bigint y2(vector<int>(op.num.begin(),op.num.begin()+(n/2)));
    y2.print();
    cout<<endl;
    bigint y1(vector<int>(op.num.begin()+(n/2),op.num.end()));
    y1.print();
    cout<<endl;
    
    bigint z0 = x1*y1;
    cout<<"z0: ";
    z0.print();
    bigint z2 = x2*y2;
    cout<<"z2: ";
    z2.print();
    // bigint z11 = x1*y2;
    // bigint z12 = y1*x2;
    // bigint z1 = z11+z12;
    bigint z1_1 = x1-x2;//p
    cout<<"z1_1: ";
    z1_1.print();
    bigint z1_2 = y2-y1;//n
    cout<<"z1_2: ";
    z1_2.print();
    bigint z1_3 = z1_1*z1_2;
    cout<<"z1_3: ";
    z1_3.print();
    bigint z1_4 = z1_3 + z0;
    cout<<"z1_4: ";
    z1_4.print();
    bigint z1 = z1_4+z2;
    cout<<"z1: ";
    z1.print();
    
    vector<int> zeroo((n/2)*2,0);
    vector<int> zero1(n/2,0);
    z0.num.insert(z0.num.begin(),zeroo.begin(),zeroo.end());
    z1.num.insert(z1.num.begin(),zero1.begin(),zero1.end());
    
    prod = z1+z0+z2;
    if(isNegative ^ op.isNegative){
        prod.isNegative = 1;
    }
    
    return prod;
}

void bigint::print(){
    
    if(num.size()==1){
        if(isNegative)
            cout<<"-";
        cout<<num.at(0);
        return;
    }
    
    if(num.size()==2){
        if(isNegative)
            cout<<"-";
        cout<<num.at(1);
        cout<<num.at(0);
        return;
    }
    
    //to remove trailing zeroes
    while (!num.empty() && num[num.size() - 1] == 0)
        num.pop_back();
    
    if(num.size() == 0){
        num.push_back(0);
    }
    
    int j;
    double i;
    
    if(isNegative)
        cout<<"-";

    cout<<num.at(num.size()-1);
    for(j=num.size()-2; j>=0; --j){
        //for human readable need to add preceding zeroes to numbers
        //eg: the element is 00083 and base 100000 then cout will give
        //83 but we need 00083. 
        if(num.at(j)<(base/10)){
            //we calculate this ampunt of zeroes by comparing the 
            //number of digits by taking the log and printing the 
            //required zeroes
            for(i=0; i<(log10(base)-1-(num.at(j)==0?0:log10(num.at(j)))); i+=1.0)
                cout<<0;
        }
        cout<<num.at(j);
    }
}

//41549622603955309777243716069997997007620439937711509062916
//41549622603955309777243716069997997007620439937711509062916
int main(){
    bigint a ("-11111");
    bigint b ("63516");
    bigint c = a*b;
    cout<<endl;
    c.print();
}