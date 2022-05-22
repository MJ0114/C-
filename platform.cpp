#include<iostream>
#include<string>
#include<fstream>
#include<vector>

#define CONSUMERFILE "consumer.dat"
#define BUSINESSFILE "business.dat"
#define BOOKFILE "book.dat"
#define CLOTHINGFILE "clothing.dat"
#define ELECGOODSFILE "elecgoods.dat"
#define SHOPPINGFILE "shopping.dat"

using namespace std;

//商品
class Commodity {
public:
    virtual double getPrice() = 0;//虚函数，获取价格
    char name[20] = { 0 };//商品名称
    double price;//价格
    int surplus;//剩余量
    int discount;//折扣
    char ID[20] = { 0 };//ID识别，实现商品与用户绑定，实现不同的用户对应商品不同
};

class Book : public Commodity {
public:
    double getPrice() { //获取价格
        return price;
    }
    Book() { discount = 7; }//构造函数，默认书本折扣为7折
    char writer[20] = { 0 };//书的作者
    void discounting() {//打折
        price = price * 0.1 * discount;
    }
};

class Clothing :public Commodity {
public:
    double getPrice() {//获取价格
        return price;
    }
    Clothing() { discount = 8; }//构造函数，默认服装折扣为8折
    char from[20] = { 0 };//服装原产国家
    void discounting() {//打折
        price = price * 0.1 * discount;
    }
};

class Elecgoods :public Commodity {
public:
    double getPrice() {//获取价格
        return price;
    }
    int yop = 0;//year of production，即生产年份
    Elecgoods() { discount = 9; }//构造函数，默认电子产品折扣为9折
    void discounting() {//打折
        price = price * 0.1 * discount;
    }
};

//购物车
class  Shopping {
public:
    char ID[20] = { 0 };//ID识别，购物车中的商品与消费者绑定，实现不同消费者的购物车唯一
    char cname[20] = { 0 };//购物车中的该商品名称
    int number = 0;//购物车中该商品书里那个
    char type[20] = { 0 };//购物车中的该商品类别，方便对不同类型的商品实现文件读取
};


//账户
class Account {
public:
    char username[20] = { 0 };//用户名
    char password[20] = { 0 };//密码
    double balance = 0;//余额
    char type[20] = { 0 };//类型
public:
    virtual char* getUserType() = 0;//虚函数，获取用户类型，在派生类中实现
};

class Business :public Account {
public:
    char type[20] = "business";//类型
    char* getUserType() {//获取用户类型，在派生类中即为商家
        return this->type;
    }
    Book book;//商家可以添加图书
    Clothing clothing;//商家可以添加服装
    Elecgoods elecgoods;//商家可以添加电子产品
    void balanceChange();//余额管理
    void addbook();//添加图书
    void addclothes();//添加服装
    void addelecgoods();//添加电子产品
    void cim();//商品信息管理
    int rgstrBsn();//注册
    int signInBsn();//登录
    void changeBsnPassword();//修改密码
};

void Business::cim() { //Commodity information management 商品信息管理
    int whichClass;
    cout << "请输入想要管理的商品类(编号)：";
    cout << "1.图书              2.服装               3.电子产品" << endl;
    cin >> whichClass;
    while (whichClass != 1 && whichClass != 2 && whichClass != 3) {
        cout << "输入有误，请重新输入：";
    }
    switch (whichClass) {
    case 1:
    {
        int have = 0;
        Book b;
        ifstream showfile(BOOKFILE, ios::in | ios::binary);
        while (showfile.read((char*)&b, sizeof(Book))) {
            if (!strcmp(b.ID, this->username)) {
                if (!have)
                    cout << "您所拥有的图书如下：" << endl;
                have++;
                cout << "书名：" << b.name << " " << "价格：" << b.price << " " << "作者：" << b.writer << " " << "剩余量：" << b.surplus << endl;
            }
        }
        if (have) {
            cout << "请选择变更类别：" << endl;
            cout << "1.全部图书打折                 2.单本图书更改" << endl;
            int choose;
            cin >> choose;
            while (choose != 1 && choose != 2) {
                cout << "输入有误，请重新输入：";
                cin >> choose;
            }
            if (choose == 1) {
                ifstream inBookFile(BOOKFILE, ios::in | ios::binary);
                while (inBookFile.read((char*)&b, sizeof(Book))) {
                    if (!strcmp(b.ID, this->username)) {
                        b.discounting();
                        vector<Book> temp;
                        Book tbook;
                        fstream iofile(BOOKFILE, ios::in | ios::binary);
                        while (iofile.read((char*)&tbook, sizeof(Book))) {  //暂存除要修改的其他用户
                            if (strcmp(tbook.name, b.name))
                                temp.push_back(tbook);
                        }
                        temp.push_back(b);//加入修改后的信息
                        iofile.close();
                        iofile.open(BOOKFILE, ios::out | ios::binary);
                        for (vector<Book>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
                            tbook = *iter;
                            iofile.write((char*)&tbook, sizeof(Book));
                        }
                        iofile.close();
                    }
                }
            }
            else {
                char bookname[20];
                cout << "请输入想要管理的图书名称：";
                cin >> bookname;
                ifstream inBookFile(BOOKFILE, ios::in | ios::binary);
                if (!inBookFile)
                    cout << "open bookfile failed!" << endl;
                while (inBookFile.read((char*)&b, sizeof(Book))) {
                    if (!strcmp(b.name, bookname) && !strcmp(b.ID, this->username)) {
                        cout << "您想变更该图书的什么信息(编号)：" << endl;
                        cout << "1.价格             2.剩余量" << endl;
                        int changemsg;
                        cin >> changemsg;
                        while (changemsg != 1 && changemsg != 2) {
                            cout << "输入有误，请重新输入：";
                            cin >> changemsg;
                        }
                        if (changemsg == 1) {
                            int newprice;
                            cout << "请输入新的价格信息：";
                            cin >> newprice;
                            while (newprice < 0) {
                                cout << "新的价格不能为负，请重新输入：" << endl;
                                cin >> newprice;
                            }
                            b.price = newprice;
                        }
                        else {
                            int newsurplus;
                            cout << "请输入新的剩余量信息：";
                            cin >> newsurplus;
                            while (newsurplus < 0) {
                                cout << "新的剩余量信息不能为负，请重新输入：" << endl;
                                cin >> newsurplus;
                            }
                            b.surplus = newsurplus;
                        }
                        vector<Book> temp;
                        Book tbook;
                        fstream iofile(BOOKFILE, ios::in | ios::binary);
                        while (iofile.read((char*)&tbook, sizeof(Book))) {  //暂存除要修改的其他用户
                            if (strcmp(tbook.name, b.name))
                                temp.push_back(tbook);
                        }
                        temp.push_back(b);//加入修改后的信息
                        iofile.close();
                        iofile.open(BOOKFILE, ios::out | ios::binary);
                        for (vector<Book>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
                            tbook = *iter;
                            iofile.write((char*)&tbook, sizeof(Book));
                        }
                        iofile.close();
                        break;
                    }
                }
            }
        }
        else
            cout << "很抱歉，您当前无图书可供更改，请添加后再进行操作！" << endl;
        break;
    }
    case 2:
    {
        int have = 0;
        Clothing c;
        ifstream showfile(CLOTHINGFILE, ios::in | ios::binary);
        while (showfile.read((char*)&c, sizeof(Clothing))) {
            if (!strcmp(c.ID, this->username)) {
                if (!have)
                    cout << "您所拥有的服装如下：" << endl;
                have++;
                cout << "服装名：" << c.name << " " << "价格：" << c.price << " " << "原产国：" << c.from << " " << "剩余量：" << c.surplus << endl;
            }
        }
        if (have) {
            cout << "请选择变更类别：" << endl;
            cout << "1.全部服装打折                 2.单件服装更改" << endl;
            int choose;
            cin >> choose;
            while (choose != 1 && choose != 2) {
                cout << "输入有误，请重新输入：";
                cin >> choose;
            }
            if (choose == 1) {
                ifstream inClothingFile(CLOTHINGFILE, ios::in | ios::binary);
                while (inClothingFile.read((char*)&c, sizeof(Clothing))) {
                    if (!strcmp(c.ID, this->username)) {
                        c.discounting();
                        vector<Clothing> temp;
                        Clothing tclothes;
                        fstream iofile(CLOTHINGFILE, ios::in | ios::binary);
                        while (iofile.read((char*)&tclothes, sizeof(Clothing))) {  //暂存除要修改的其他用户
                            if (strcmp(tclothes.name, c.name))
                                temp.push_back(tclothes);
                        }
                        temp.push_back(c);//加入修改后的信息
                        iofile.close();
                        iofile.open(CLOTHINGFILE, ios::out | ios::binary);
                        for (vector<Clothing>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
                            tclothes = *iter;
                            iofile.write((char*)&tclothes, sizeof(Clothing));
                        }
                        iofile.close();
                    }
                }
            }
            else {
                char clothesname[20];
                cout << "请输入想要管理的服装名称：";
                cin >> clothesname;
                ifstream inClothingFile(CLOTHINGFILE, ios::in | ios::binary);
                if (!inClothingFile)
                    cout << "open clothesfile failed!" << endl;
                while (inClothingFile.read((char*)&c, sizeof(Clothing))) {
                    if (!strcmp(c.name, clothesname) && !strcmp(c.ID, this->username)) {
                        cout << "您想变更该服装的什么信息(编号)：";
                        cout << "1.价格                 2.剩余量" << endl;
                        int changemsg;
                        cin >> changemsg;
                        while (changemsg != 1 && changemsg != 2) {
                            cout << "输入有误，请重新输入：";
                            cin >> changemsg;
                        }
                        if (changemsg == 1) {
                            int newprice;
                            cout << "请输入新的价格信息：";
                            cin >> newprice;
                            while (newprice < 0) {
                                cout << "新的价格不能为负，请重新输入：" << endl;
                                cin >> newprice;
                            }
                            c.price = newprice;
                        }
                        else {
                            int newsurplus;
                            cout << "请输入新的剩余量信息：";
                            cin >> newsurplus;
                            while (newsurplus < 0) {
                                cout << "新的剩余量信息不能为负，请重新输入：" << endl;
                                cin >> newsurplus;
                            }
                            c.surplus = newsurplus;
                        }
                        vector<Clothing> temp;
                        Clothing tclothes;
                        fstream iofile(CLOTHINGFILE, ios::in | ios::binary);
                        while (iofile.read((char*)&tclothes, sizeof(Clothing))) {  //暂存除要修改的其他用户
                            if (strcmp(tclothes.name, c.name))
                                temp.push_back(tclothes);
                        }
                        temp.push_back(c);//加入修改后的信息
                        iofile.close();
                        iofile.open(CLOTHINGFILE, ios::out | ios::binary);
                        for (vector<Clothing>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
                            tclothes = *iter;
                            iofile.write((char*)&tclothes, sizeof(Clothing));
                        }
                        iofile.close();
                        break;
                    }
                }
            }
        }
        else
            cout << "很抱歉，您当前暂无服装可供更改，请添加后再进行操作！" << endl;
        break;
    }
    case 3:
    {
        int have = 0;
        Elecgoods e;
        ifstream showfile(ELECGOODSFILE, ios::in | ios::binary);
        while (showfile.read((char*)&e, sizeof(Elecgoods))) {
            if (!strcmp(e.ID, this->username)) {
                if(!have)
                    cout << "您所拥有的电子产品如下：" << endl;
                have++;
                cout << "电子产品名：" << e.name << " " << "价格：" << e.price << " " << "生产年份：" << e.yop << " " << "剩余量：" << e.surplus << endl;
            }
        }
        if (have) {
            cout << "请选择变更类别：" << endl;
            cout << "1.全部电子产品打折                 2.单件电子产品更改" << endl;
            int choose;
            cin >> choose;
            while (choose != 1 && choose != 2) {
                cout << "输入有误，请重新输入：";
                cin >> choose;
            }
            if (choose == 1) {
                ifstream inElecgoodsFile(ELECGOODSFILE, ios::in | ios::binary);
                while (inElecgoodsFile.read((char*)&e, sizeof(Elecgoods))) {
                    if (!strcmp(e.ID, this->username)) {
                        e.discounting();
                        vector<Elecgoods> temp;
                        Elecgoods telecgoods;
                        fstream iofile(ELECGOODSFILE, ios::in | ios::binary);
                        while (iofile.read((char*)&telecgoods, sizeof(Elecgoods))) {  //暂存除要修改的其他用户
                            if (strcmp(telecgoods.name, e.name))
                                temp.push_back(telecgoods);
                        }
                        temp.push_back(e);//加入修改后的信息
                        iofile.close();
                        iofile.open(ELECGOODSFILE, ios::out | ios::binary);
                        for (vector<Elecgoods>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
                            telecgoods = *iter;
                            iofile.write((char*)&telecgoods, sizeof(Elecgoods));
                        }
                        iofile.close();
                    }
                }
            }
            else {
                char elecgoodsname[20];
                cout << "请输入想要管理的电子产品名称：";
                cin >> elecgoodsname;
                ifstream inElecgoodsFile(ELECGOODSFILE, ios::in | ios::binary);
                if (!inElecgoodsFile)
                    cout << "open clothesfile failed!" << endl;
                while (inElecgoodsFile.read((char*)&e, sizeof(Elecgoods))) {
                    if (!strcmp(e.name, elecgoodsname) && !strcmp(e.ID, this->username)) {
                        cout << "您想变更该电子产品的什么信息(编号)：";
                        cout << "1.价格                    2.剩余量" << endl;
                        int changemsg;
                        cin >> changemsg;
                        while (changemsg != 1 && changemsg != 2) {
                            cout << "输入有误，请重新输入：";
                            cin >> changemsg;
                        }
                        if (changemsg == 1) {
                            int newprice;
                            cout << "请输入新的价格信息：";
                            cin >> newprice;
                            while (newprice < 0) {
                                cout << "新的价格不能为负，请重新输入：" << endl;
                                cin >> newprice;
                            }
                            e.price = newprice;
                        }
                        else {
                            int newsurplus;
                            cout << "请输入新的剩余量信息：";
                            cin >> newsurplus;
                            while (newsurplus < 0) {
                                cout << "新的剩余量信息不能为负，请重新输入：" << endl;
                                cin >> newsurplus;
                            }
                            e.surplus = newsurplus;
                        }
                        vector<Elecgoods> temp;
                        Elecgoods telecgoods;
                        fstream iofile(ELECGOODSFILE, ios::in | ios::binary);
                        while (iofile.read((char*)&telecgoods, sizeof(Elecgoods))) {  //暂存除要修改的其他用户
                            if (strcmp(telecgoods.name, e.name))
                                temp.push_back(telecgoods);
                        }
                        temp.push_back(e);//加入修改后的信息
                        iofile.close();
                        iofile.open(ELECGOODSFILE, ios::out | ios::binary);
                        for (vector<Elecgoods>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
                            telecgoods = *iter;
                            iofile.write((char*)&telecgoods, sizeof(Elecgoods));
                        }
                        iofile.close();
                        break;
                    }
                }
            }
        }
        else
            cout << "很抱歉，您当前暂无电子产品可供更改，请添加后再进行操作！" << endl;
        break;
    }
    default:
        break;
    }
}

void Business::addbook() {//添加图书
    int add;
    cout << "请输入图书名称：";
    cin >> this->book.name;
    while (strlen(this->book.name) > 20) {
        cout << "图书名称超限，请重新输入：" << endl;
        cin >> this->book.name;
    }
    cout << "请输入图书价格：";
    cin >> this->book.price;
    while (this->book.price<0) {
        cout << "图书价格不能为负，请重新输入：" << endl;
        cin >> this->book.price;
    }
    cout << "请输入作者姓名：";
    cin >> this->book.writer;
    while (strlen(this->book.writer) > 20) {
        cout << "图书作者名超限，请重新输入：" << endl;
        cin >> this->book.writer;
    }
    cout << "请输入添加数量：";
    cin >> add;
    while (add <= 0) {
        cout << "添加数量需为正整数，请重新输入：" << endl;
        cin >> add;
    }
    this->book.surplus = 0;
    this->book.surplus += add;
    strcpy_s(this->book.ID, 20, this->username);
    fstream iofile(BOOKFILE, ios::app | ios::binary);
    iofile.write((char*)&(this->book), sizeof(Book));
    iofile.close();
}

void Business::addclothes() {//添加服装
    int add;
    cout << "请输入服装品牌：";
    cin >> this->clothing.name;
    while (strlen(this->clothing.name) > 20) {
        cout << "服装名称超限，请重新输入：" << endl;
        cin >> this->clothing.name;
    }
    cout << "请输入服装价格：";
    cin >> this->clothing.price;
    while (this->clothing.price < 0) {
        cout << "服装价格不能为负，请重新输入：" << endl;
        cin >> this->clothing.price;
    }
    cout << "请输入原产国家：";
    cin >> this->clothing.from;
    while (strlen(this->clothing.from) > 20) {
        cout << "原产国家名称位数超限，请重新输入：" << endl;
        cin >> this->clothing.from;
    }
    cout << "请输入添加数量：";
    cin >> add;
    while (add <= 0) {
        cout << "添加数量需为正整数，请重新输入：" << endl;
        cin >> add;
    }
    this->clothing.surplus = 0;
    this->clothing.surplus += add;
    strcpy_s(this->clothing.ID, 20, this->username);
    fstream iofile(CLOTHINGFILE, ios::app | ios::binary);
    iofile.write((char*)&(this->clothing), sizeof(Clothing));
    iofile.close();
}

void Business::addelecgoods() {//添加电子产品
    int add;
    cout << "请输入电子名称：";
    cin >> this->elecgoods.name;
    while (strlen(this->elecgoods.name) > 20) {
        cout << "电子产品名称超限，请重新输入：" << endl;
        cin >> this->elecgoods.name;
    }
    cout << "请输入电子价格：";
    cin >> this->elecgoods.price;
    while (this->elecgoods.price < 0) {
        cout << "电子产品价格不能为负，请重新输入：" << endl;
        cin >> this->elecgoods.price;
    }
    cout << "请输入电子产品生产年份：";
    cin >> this->elecgoods.yop;
    cout << "请输入添加数量：";
    cin >> add;
    while (add <= 0) {
        cout << "添加数量需为正整数，请重新输入：" << endl;
        cin >> add;
    }
    this->elecgoods.surplus = 0;
    this->elecgoods.surplus += add;
    strcpy_s(this->elecgoods.ID, 20, this->username);
    fstream iofile(ELECGOODSFILE, ios::app | ios::binary);
    iofile.write((char*)&(this->elecgoods), sizeof(Elecgoods));
    iofile.close();
}

void Business::balanceChange() {//余额管理
    Business b1, tempb;
    int select;
    cout << "请输入您想进行的操作(编号即可)：" << endl;
    cout << "1.查询              2.充值              3.消费" << endl;
    cin >> select;
    while (select < 1 || select > 3) {
        cout << "输入有误，请重新输入" << endl;
        cin >> select;
    }
    switch (select) {
    case 1:
        cout << "您当前余额为：" << this->balance << endl; break;
    case 2:
    {
        int addmoney;
        cout << "请输入充值金额:";
        cin >> addmoney;
        while (addmoney < 0) {
            cout << "充值金额不能为负，请重新输入：";
            cin >> addmoney;
        }
        this->balance += addmoney;
        vector<Business> temp;
        fstream iofile(BUSINESSFILE, ios::in | ios::binary);
        while (iofile.read((char*)&tempb, sizeof(Business))) {  //暂存除要修改余额的其他用户
            if (strcmp(tempb.username, this->username))
                temp.push_back(tempb);
        }
        temp.push_back(*this);
        iofile.close();
        iofile.open(BUSINESSFILE, ios::out | ios::binary);
        for (vector<Business>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
            tempb = *iter;
            iofile.write((char*)&tempb, sizeof(Business));
        }
        iofile.close();
        break;
    }
    case 3:
    {
        int submoney;
        cout << "请输入消费金额:";
        cin >> submoney;
        while (submoney < 0) {
            cout << "消费金额不能为负，请重新输入：";
            cin >> submoney;
        }
        balance -= submoney;
        vector<Business> temp;
        fstream iofile(BUSINESSFILE, ios::in | ios::binary);
        while (iofile.read((char*)&tempb, sizeof(Business))) {  //暂存除要修改余额的其他用户
            if (strcmp(tempb.username, this->username))
                temp.push_back(tempb);
        }
        temp.push_back(*this);
        iofile.close();
        iofile.open(BUSINESSFILE, ios::out | ios::binary);
        for (vector<Business>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
            tempb = *iter;
            iofile.write((char*)&tempb, sizeof(Business));
        }
        iofile.close();
        break;
    }
    default:
        break;
    }
}

class Consumer :public Account {
public:
    char type[20] = "consumer";//类型
    double sumMoney = 0;//生成订单后的待支付金额，默认为0
    char* getUserType() {//获取用户类型，在派生类中即为消费者
        return this->type;
    }
    Consumer() { sumMoney = 0; }//构造函数，默认待支付金额为0
    void balanceChange();//余额管理 
    Shopping s;//购物车
    void ShoppingCartChange();//购物车管理
    void viewShoppingCart();//查看购物车
    double orderGeneration();//订单生成
    int onlinePayment();//网上支付，   返回值0：支付失败     返回值1：支付成功
    int rgstrCsm();//注册
    int signInCsm();//登录
    void showCommodity();//平台商品信息展示
    int searchCommodity();//搜索平台商品
    void changeCsmPassword();//修改密码
    int coo();//cancellation of order，即取消订单
};

void Consumer::balanceChange() {//余额管理
    Consumer c1, tempc;
    int select;
    cout << "请输入您想进行的操作(编号即可)：" << endl;
    cout << "1.查询              2.充值              3.消费" << endl;
    cin >> select;
    while (select < 1 || select > 3) {
        cout << "输入有误，请重新输入" << endl;
        cin >> select;
    }
    switch (select) {
    case 1:
        cout << "您当前余额为：" << balance << endl; break;
    case 2:
    {
        int addmoney;
        cout << "请输入充值金额:";
        cin >> addmoney;
        while (addmoney < 0) {
            cout << "充值金额不能为负，请重新输入：";
            cin >> addmoney;
        }
        balance += addmoney;
        vector<Consumer> temp;
        fstream iofile(CONSUMERFILE, ios::in | ios::binary);
        while (iofile.read((char*)&tempc, sizeof(Consumer))) {  //暂存除要修改余额的其他用户
            if (strcmp(tempc.username, this->username))
                temp.push_back(tempc);
        }
        temp.push_back(*this);
        iofile.close();
        iofile.open(CONSUMERFILE, ios::out | ios::binary);
        for (vector<Consumer>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
            tempc = *iter;
            iofile.write((char*)&tempc, sizeof(Consumer));
        }
        iofile.close();
        break;
    }
    case 3:
    {
        int submoney;
        cout << "请输入消费金额：";
        cin >> submoney;
        while (submoney < 0) {
            cout << "消费金额不能为负，请重新输入：";
            cin >> submoney;
        }
        balance -= submoney;
        vector<Consumer> temp;
        fstream iofile(CONSUMERFILE, ios::in | ios::binary);
        while (iofile.read((char*)&tempc, sizeof(Consumer))) {  //暂存除要修改余额的其他用户
            if (strcmp(tempc.username, this->username))
                temp.push_back(tempc);
        }
        temp.push_back(*this);//加入修改后的用户信息
        iofile.close();
        iofile.open(CONSUMERFILE, ios::out | ios::binary);
        for (vector<Consumer>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
            tempc = *iter;
            iofile.write((char*)&tempc, sizeof(Consumer));
        }
        iofile.close();
        break;
    }
    default:
        break;
    }
}

void Consumer::ShoppingCartChange() {//购物车管理
    int select = 0;
    int isBreak = 0;
    int nosave = 0;//用于在商品已有时在新添加时判断覆盖
    int maxnum = 0;//可更改的最大量
    cout << "请选择待变更购物车中的商品种类：" << endl;
    cout << "1.图书               2.服装               3.电子产品" << endl;
    cin >> select;
    while (select != 1 && select != 2 && select != 3) {
        cout << "输入有误，请重新输入：";
        cin >> select;
    }
    switch (select) {
    case 1:
    {
        int have = 0;
        Book b;
        Shopping s, temps;
        //展示该用户的购物车中的物品
        cout << "您购物车中中现有图书为：" << endl;
        ifstream shoppingfile(SHOPPINGFILE, ios::in | ios::binary);
        while (shoppingfile.read((char*)&s, sizeof(Shopping))) {
            if (!strcmp(s.type, "book") && !strcmp(s.ID, this->username)) {
                have++;
                cout << "书名：" << s.cname << " " << "数量：" << s.number << endl;
            }
        }
        cout << endl;
        shoppingfile.close();

        cout << "请选择购物车变更类型：" << endl;
        cout << "1.变更现有购物车中图书           2.增加图书至购物车" << endl;
        int whichtype;
        cin >> whichtype;
        while (whichtype != 1 && whichtype != 2) {
            cout << "输入有误，请重新输入：";
            cin >> whichtype;
        }
        if (whichtype == 1) {  //1.变更现有购物车中图书 
            if (have) {
                char name[20];
                cout << "请输入想在购物车中变更的图书名称：";
                cin >> name;
                shoppingfile.open(SHOPPINGFILE, ios::in | ios::binary);
                while (shoppingfile.read((char*)&s, sizeof(Shopping))) {
                    if (!strcmp(s.cname, name) && !strcmp(s.ID, this->username)) {
                        int cg;
                        strcpy_s(s.ID, 20, this->username);
                        strcpy_s(s.cname, 20, name);
                        cout << "请选择变更类型：" << endl;
                        cout << "1.增加            2.减少             3.更改" << endl;
                        cin >> cg;
                        while (cg != 1 && cg != 2 && cg != 3) {
                            cout << "输入有误，请重新输入：";
                            cin >> cg;
                        }
                        fstream checkfile(BOOKFILE, ios::in | ios::binary);
                        while (checkfile.read((char*)&b, sizeof(Book))) {
                            if (!strcmp(b.name, s.cname)) {
                                maxnum = b.surplus;
                                break;
                            }
                        }
                        checkfile.close();
                        if (cg == 1) {
                            int addnum;
                            cout << "请输入添加的数量：";
                            cin >> addnum;
                            while (addnum <= 0) {
                                cout << "添加数量需为正数，请重新输入：" << endl;
                                cin >> addnum;
                            }
                            while ((s.number + addnum) > maxnum) {
                                cout << "商家的该商品的剩余量为：" << maxnum << endl;
                                cout << "您所添加到购物车的产品数量已超过商家产品剩余量，请重新输入：" << endl;
                                cin >> addnum;
                            }
                            s.number += addnum;
                        }
                        else if (cg == 2) {
                            int subnum;
                            cout << "请输入减少的数量：";
                            cin >> subnum;
                            while (subnum <= 0) {
                                cout << "减少数量需为正数，请重新输入：" << endl;
                                cin >> subnum;
                            }
                            s.number -= subnum;
                            if (s.number < 0) {
                                cout << "减少数量已超限，现已重置为0！" << endl;
                                s.number = 0;
                            }
                        }
                        else {
                            int newnum;
                            cout << "请输入新的数量：";
                            cin >> newnum;
                            while (newnum <= 0) {
                                cout << "新的数量需为正数，请重新输入：" << endl;
                                cin >> newnum;
                            }
                            while (newnum > maxnum) {
                                cout << "商家的该商品的剩余量为：" << maxnum << endl;
                                cout << "新更改的数量已超过商家产品剩余量，请重新输入：" << endl;
                                cin >> newnum;
                            }
                            s.number = newnum;
                        }
                        strcpy_s(s.type, 20, "book");
                        vector<Shopping> temp;
                        fstream iofile(SHOPPINGFILE, ios::in | ios::binary);
                        while (iofile.read((char*)&temps, sizeof(Shopping))) {
                            if (strcmp(temps.ID, this->username) || strcmp(temps.cname, name))
                                if (temps.number)
                                    temp.push_back(temps);
                        }
                        if (s.number)//这里也需要判断更改后的数量是否为0
                            temp.push_back(s);
                        iofile.close();
                        iofile.open(SHOPPINGFILE, ios::out | ios::binary);
                        for (vector<Shopping>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
                            temps = *iter;
                            iofile.write((char*)&temps, sizeof(Shopping));
                        }
                        iofile.close();
                        break;
                    }
                }
            }
            else
                cout << "您购物车中暂无图书类商品，请添加后重试！" << endl;
        }
        else { //2.增加图书至购物车

            //展示商家有什么该类商品,即可添加的图书
            cout << "商品库中现有图书为：" << endl;
            ifstream showfile(BOOKFILE, ios::in | ios::binary);
            while (showfile.read((char*)&b, sizeof(Book))) {
                cout << "书名：" << b.name << " " << "价格：" << b.price << " " << "作者：" << b.writer << " " << "剩余量：" << b.surplus << endl;
            }
            cout << endl;
            showfile.close();

            char name[20];
            cout << "请输入想在购物车中增加的图书名称：";
            cin >> name;
            showfile.open(BOOKFILE, ios::in | ios::binary);
            while (showfile.read((char*)&b, sizeof(Book))) {//先看商家有没有这本书
                if (!strcmp(b.name, name)) {
                    fstream iofile(SHOPPINGFILE, ios::in | ios::binary);
                    while (iofile.read((char*)&s, sizeof(Shopping))) {//再看购物车中是否已经有了这本书
                        if (!strcmp(s.cname, name)&&!strcmp(s.ID,this->username)) {
                            nosave = 1;
                            cout << "请输入添加数量：";
                            int addnum;
                            cin >> addnum;
                            while (addnum <= 0) {
                                cout << "添加数量需为正数，请重新输入：" << endl;
                                cin >> addnum;
                            }
                            fstream checkfile(BOOKFILE, ios::in | ios::binary);
                            while (checkfile.read((char*)&b, sizeof(Book))) {
                                if (!strcmp(b.name, s.cname)) {
                                    maxnum = b.surplus;
                                    break;
                                }
                            }
                            checkfile.close();
                            while ((s.number + addnum) > maxnum) {
                                cout << "商家的该商品的剩余量为：" << maxnum << endl;
                                cout << "您所添加到购物车的产品数量已超过商家产品剩余量，请重新输入：" << endl;
                                cin >> addnum;
                            }
                            s.number += addnum;
                            isBreak = 1;
                            vector<Shopping> temp;
                            fstream tfile(SHOPPINGFILE, ios::in | ios::binary);
                            while (iofile.read((char*)&temps, sizeof(Shopping))) {  //暂存除要修改余额的其他用户
                                if (strcmp(temps.cname, name) || strcmp(temps.ID, this->username))
                                    temp.push_back(temps);
                            }
                            temp.push_back(s);
                            tfile.close();
                            tfile.open(SHOPPINGFILE, ios::out | ios::binary);
                            for (vector<Shopping>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
                                temps = *iter;
                                tfile.write((char*)&temps, sizeof(Shopping));
                            }
                            tfile.close();
                            break;
                        }
                    }
                    iofile.close();
                    if (!isBreak) {
                        strcpy_s(s.cname, 20, b.name);
                        strcpy_s(s.ID, 20, this->username);
                        cout << "请输入添加数量：";
                        int addnum;
                        cin >> addnum;
                        while (addnum <= 0) {
                            cout << "添加数量需为正数，请重新输入：" << endl;
                            cin >> addnum;
                        }
                        while (addnum > b.surplus) {
                            cout << "商家的该商品的剩余量为：" << b.surplus << endl;
                            cout << "您所添加到购物车的产品数量已超过商家产品剩余量，请重新输入：" << endl;
                            cin >> addnum;
                        }
                        s.number = addnum;
                        strcpy_s(s.type, 20, "book");
                        isBreak = 0;
                    }
                    iofile.open(SHOPPINGFILE, ios::app | ios::binary);
                    if (!nosave)
                        iofile.write((char*)&s, sizeof(Shopping));
                    iofile.close();
                    nosave = 0;//重置为0
                    break;
                }
            }
        }
        break;
    }
    case 2:
    {
        int have = 0;
        Clothing c;
        Shopping s, temps;
        //展示该用户的购物车中的物品
        cout << "您购物车中中现有服装为：" << endl;
        ifstream shoppingfile(SHOPPINGFILE, ios::in | ios::binary);
        while (shoppingfile.read((char*)&s, sizeof(Shopping))) {
            if (!strcmp(s.type, "clothes") && !strcmp(s.ID, this->username)) {
                have++;
                cout << "服装名：" << s.cname << " " << "数量：" << s.number << endl;
            }   
        }
        cout << endl;
        shoppingfile.close();

        cout << "请选择购物车变更类型：" << endl;
        cout << "1.变更现有购物车中服装           2.增加服装至购物车" << endl;
        int whichtype;
        cin >> whichtype;
        while (whichtype != 1 && whichtype != 2) {
            cout << "输入有误，请重新输入：";
            cin >> whichtype;
        }
        if (whichtype == 1) {  //1.变更现有购物车中服装 
            if (have) {
                char name[40];
                cout << "请输入想在购物车中变更的服装名称：";
                cin >> name;
                shoppingfile.open(SHOPPINGFILE, ios::in | ios::binary);
                while (shoppingfile.read((char*)&s, sizeof(Shopping))) {
                    if (!strcmp(s.cname, name) && !strcmp(s.ID, this->username)) {
                        int cg;
                        strcpy_s(s.ID, 20, this->username);
                        strcpy_s(s.cname, 20, name);
                        cout << "请选择变更类型：" << endl;
                        cout << "1.增加            2.减少             3.更改" << endl;
                        cin >> cg;
                        while (cg != 1 && cg != 2 && cg != 3) {
                            cout << "输入有误，请重新输入：";
                            cin >> cg;
                        }
                        fstream checkfile(CLOTHINGFILE, ios::in | ios::binary);
                        while (checkfile.read((char*)&c, sizeof(Clothing))) {
                            if (!strcmp(c.name, s.cname)) {
                                maxnum = c.surplus;
                                break;
                            }
                        }
                        checkfile.close();
                        if (cg == 1) {
                            int addnum;
                            cout << "请输入添加的数量：";
                            cin >> addnum;
                            while (addnum <= 0) {
                                cout << "添加数量需为正数，请重新输入：" << endl;
                                cin >> addnum;
                            }
                            while ((s.number + addnum) > maxnum) {
                                cout << "商家的该商品的剩余量为：" << maxnum << endl;
                                cout << "您所添加到购物车的产品数量已超过商家产品剩余量，请重新输入：" << endl;
                                cin >> addnum;
                            }
                            s.number += addnum;
                        }
                        else if (cg == 2) {
                            int subnum;
                            cout << "请输入减少的数量：";
                            cin >> subnum;
                            while (subnum <= 0) {
                                cout << "减少数量需为正数，请重新输入：" << endl;
                                cin >> subnum;
                            }
                            s.number -= subnum;
                            if (s.number < 0) {
                                cout << "减少数量已超限，现已重置为0！" << endl;
                                s.number = 0;
                            }
                        }
                        else {
                            int newnum;
                            cout << "请输入新的数量：";
                            cin >> newnum;
                            while (newnum <= 0) {
                                cout << "新的数量需为正数，请重新输入：" << endl;
                                cin >> newnum;
                            }
                            while (newnum > maxnum) {
                                cout << "商家的该商品的剩余量为：" << maxnum << endl;
                                cout << "新更改的数量已超过商家产品剩余量，请重新输入：" << endl;
                                cin >> newnum;
                            }
                            s.number = newnum;
                        }
                        strcpy_s(s.type, 20, "clothes");
                        vector<Shopping> temp;
                        fstream iofile(SHOPPINGFILE, ios::in | ios::binary);
                        while (iofile.read((char*)&temps, sizeof(Shopping))) {
                            if (strcmp(temps.ID, this->username) || strcmp(temps.cname, name))
                                if (temps.number)//数量为0就不必再存进去
                                    temp.push_back(temps);
                        }
                        if (s.number)//这里也需要判断更改后的数量是否为0
                            temp.push_back(s);
                        iofile.close();
                        iofile.open(SHOPPINGFILE, ios::out | ios::binary);
                        for (vector<Shopping>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
                            temps = *iter;
                            iofile.write((char*)&temps, sizeof(Shopping));
                        }
                        iofile.close();
                        break;
                    }
                }
            }
            else
                cout << "您购物车中暂无服装类商品，请添加后重试！" << endl;
        }
        else { //2.增加服装至购物车

            //展示商家有什么该类商品,即可添加的服装
            cout << "商品库中现有服装为：" << endl;
            ifstream showfile(CLOTHINGFILE, ios::in | ios::binary);
            while (showfile.read((char*)&c, sizeof(Clothing))) {
                cout << "服装名：" << c.name << " " << "价格：" << c.price << " " << "原产国：" << c.from << " " << "剩余量：" << c.surplus << endl;
            }
            cout << endl;
            showfile.close();

            char name[20];
            cout << "请输入想在购物车中增加的服装名称：";
            cin >> name;
            showfile.open(CLOTHINGFILE, ios::in | ios::binary);
            while (showfile.read((char*)&c, sizeof(Clothing))) {//先看商家有没有这件服装
                if (!strcmp(c.name, name)) {
                    fstream iofile(SHOPPINGFILE, ios::in | ios::binary);
                    while (iofile.read((char*)&s, sizeof(Shopping))) {//再看购物车中是否已经有了这件服装
                        if (!strcmp(s.cname, name)&&!strcmp(s.ID,this->username)) {
                            nosave = 1;
                            cout << "请输入添加数量：";
                            int addnum;
                            cin >> addnum;
                            while (addnum <= 0) {
                                cout << "添加数量需为正数，请重新输入：" << endl;
                                cin >> addnum;
                            }
                            fstream checkfile(CLOTHINGFILE, ios::in | ios::binary);
                            while (checkfile.read((char*)&c, sizeof(Clothing))) {
                                if (!strcmp(c.name, s.cname)) {
                                    maxnum = c.surplus;
                                    break;
                                }
                            }
                            checkfile.close();
                            while ((s.number + addnum) > maxnum) {
                                cout << "商家的该商品的剩余量为：" << maxnum << endl;
                                cout << "您所添加到购物车的产品数量已超过商家产品剩余量，请重新输入：" << endl;
                                cin >> addnum;
                            }
                            s.number += addnum;
                            isBreak = 1;
                            vector<Shopping> temp;
                            fstream tfile(SHOPPINGFILE, ios::in | ios::binary);
                            while (iofile.read((char*)&temps, sizeof(Shopping))) {  //暂存除要修改余额的其他用户
                                if (strcmp(temps.cname, name) || strcmp(temps.ID, this->username))
                                    temp.push_back(temps);
                            }
                            temp.push_back(s);
                            tfile.close();
                            tfile.open(SHOPPINGFILE, ios::out | ios::binary);
                            for (vector<Shopping>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
                                temps = *iter;
                                tfile.write((char*)&temps, sizeof(Shopping));
                            }
                            tfile.close();
                            break;
                        }
                    }
                    iofile.close();
                    if (!isBreak) {
                        strcpy_s(s.cname, 20, c.name);
                        strcpy_s(s.ID, 20, this->username);
                        cout << "请输入添加数量：";
                        int addnum;
                        cin >> addnum;
                        while (addnum <= 0) {
                            cout << "添加数量需为正数，请重新输入：" << endl;
                            cin >> addnum;
                        }
                        while (addnum > c.surplus) {
                            cout << "商家的该商品的剩余量为：" << c.surplus << endl;
                            cout << "您所添加到购物车的产品数量已超过商家产品剩余量，请重新输入：" << endl;
                            cin >> addnum;
                        }
                        s.number = addnum;
                        strcpy_s(s.type, 20, "clothes");
                    }
                    iofile.open(SHOPPINGFILE, ios::app | ios::binary);
                    if (!nosave)
                        iofile.write((char*)&s, sizeof(Shopping));
                    iofile.close();
                    nosave = 0;
                    break;
                }
            }
        }
        break;
    }
    case 3:
    {
        int have = 0;
        Elecgoods e;
        Shopping s, temps;
        //展示该用户的购物车中的物品
        cout << "您购物车中中现有电子产品为：" << endl;
        ifstream shoppingfile(SHOPPINGFILE, ios::in | ios::binary);
        while (shoppingfile.read((char*)&s, sizeof(Shopping))) {
            if (!strcmp(s.type, "elecgoods") && !strcmp(this->username, s.ID)) {
                have++;
                cout << "电子产品名：" << s.cname << " " << "数量：" << s.number << endl;
            }
        }
        cout << endl;
        shoppingfile.close();

        cout << "请选择购物车变更类型：" << endl;
        cout << "1.变更现有购物车中电子产品           2.增加电子产品至购物车" << endl;
        int whichtype;
        cin >> whichtype;
        while (whichtype != 1 && whichtype != 2) {
            cout << "输入有误，请重新输入：";
            cin >> whichtype;
        }
        if (whichtype == 1) {  //1.变更现有购物车中电子产品 
            if (have) {
                char name[20];
                cout << "请输入想在购物车中变更的电子产品名称：";
                cin >> name;
                shoppingfile.open(SHOPPINGFILE, ios::in | ios::binary);
                while (shoppingfile.read((char*)&s, sizeof(Shopping))) {
                    if (!strcmp(s.cname, name) && !strcmp(s.ID, this->username)) {
                        int cg;
                        strcpy_s(s.ID, 20, this->username);
                        strcpy_s(s.cname, 20, name);
                        cout << "请选择变更类型：" << endl;
                        cout << "1.增加            2.减少             3.更改" << endl;
                        cin >> cg;
                        while (cg != 1 && cg != 2 && cg != 3) {
                            cout << "输入有误，请重新输入：";
                            cin >> cg;
                        }
                        fstream checkfile(ELECGOODSFILE, ios::in | ios::binary);
                        while (checkfile.read((char*)&e, sizeof(Elecgoods))) {
                            if (!strcmp(e.name, s.cname)) {
                                maxnum = e.surplus;
                                break;
                            }
                        }
                        checkfile.close();
                        if (cg == 1) {
                            int addnum;
                            cout << "请输入添加的数量：";
                            cin >> addnum;
                            while (addnum <= 0) {
                                cout << "添加数量需为正数，请重新输入：" << endl;
                                cin >> addnum;
                            }
                            while ((s.number + addnum) > maxnum) {
                                cout << "商家的该商品的剩余量为：" << maxnum << endl;
                                cout << "您所添加到购物车的产品数量已超过商家产品剩余量，请重新输入：" << endl;
                                cin >> addnum;
                            }
                            s.number += addnum;
                        }
                        else if (cg == 2) {
                            int subnum;
                            cout << "请输入减少的数量：";
                            cin >> subnum;
                            while (subnum <= 0) {
                                cout << "减少数量需为正数，请重新输入：" << endl;
                                cin >> subnum;
                            }
                            s.number -= subnum;
                            if (s.number < 0) {
                                cout << "减少数量已超限，现已重置为0！" << endl;
                                s.number = 0;
                            }
                        }
                        else {
                            int newnum;
                            cout << "请输入新的数量：";
                            cin >> newnum;
                            while (newnum <= 0) {
                                cout << "新的数量需为正数，请重新输入：" << endl;
                                cin >> newnum;
                            }
                            while (newnum > maxnum) {
                                cout << "商家的该商品的剩余量为：" << maxnum << endl;
                                cout << "新更改的数量已超过商家产品剩余量，请重新输入：" << endl;
                                cin >> newnum;
                            }
                            s.number = newnum;
                        }
                        strcpy_s(s.type, 20, "elecgoods");
                        vector<Shopping> temp;
                        fstream iofile(SHOPPINGFILE, ios::in | ios::binary);
                        while (iofile.read((char*)&temps, sizeof(Shopping))) {
                            if (strcmp(temps.ID, this->username) || strcmp(temps.cname, name))
                                if (temps.number)//数量为0就不必再存进去
                                    temp.push_back(temps);
                        }
                        if (s.number)//这里也需要判断更改后的数量是否为0
                            temp.push_back(s);
                        iofile.close();
                        iofile.open(SHOPPINGFILE, ios::out | ios::binary);
                        for (vector<Shopping>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
                            temps = *iter;
                            iofile.write((char*)&temps, sizeof(Shopping));
                        }
                        iofile.close();
                        break;
                    }
                }
            }
            else
                cout << "您购物车中暂无服装类商品，请添加后重试！" << endl;
        }
        else { //2.增加电子产品至购物车

            //展示商家有什么该类商品,即可添加的电子产品
            cout << "商品库中现有电子产品为：" << endl;
            ifstream showfile(ELECGOODSFILE, ios::in | ios::binary);
            while (showfile.read((char*)&e, sizeof(Elecgoods))) {
                cout << "电子产品名：" << e.name << " " << "价格：" << e.price << " " << "生产年份：" << e.yop << " " << "剩余量：" << e.surplus << endl;
            }
            cout << endl;
            showfile.close();

            char name[20];
            cout << "请输入想在购物车中增加的服装名称：";
            cin >> name;
            showfile.open(ELECGOODSFILE, ios::in | ios::binary);
            while (showfile.read((char*)&e, sizeof(Elecgoods))) {//先看商家有没有这个电子产品
                if (!strcmp(e.name, name)) {
                    fstream iofile(SHOPPINGFILE, ios::in | ios::binary);
                    while (iofile.read((char*)&s, sizeof(Shopping))) {//再看购物车中是否已经有了这个电子产品
                        if (!strcmp(s.cname, name)&&!strcmp(s.ID,this->username)) {
                            nosave = 1;
                            cout << "请输入添加数量：";
                            int addnum;
                            cin >> addnum;
                            while (addnum <= 0) {
                                cout << "添加数量需为正数，请重新输入：" << endl;
                                cin >> addnum;
                            }
                            fstream checkfile(ELECGOODSFILE, ios::in | ios::binary);
                            while (checkfile.read((char*)&e, sizeof(Elecgoods))) {
                                if (!strcmp(e.name, s.cname)) {
                                    maxnum = e.surplus;
                                    break;
                                }
                            }
                            checkfile.close();
                            while ((s.number + addnum) > maxnum) {
                                cout << "商家的该商品的剩余量为：" << maxnum << endl;
                                cout << "您所添加到购物车的产品数量已超过商家产品剩余量，请重新输入：" << endl;
                                cin >> addnum;
                            }
                            s.number += addnum;
                            isBreak = 1;
                            vector<Shopping> temp;
                            fstream tfile(SHOPPINGFILE, ios::in | ios::binary);
                            while (iofile.read((char*)&temps, sizeof(Shopping))) {  //暂存除要修改余额的其他用户
                                if (strcmp(temps.cname, name) || strcmp(temps.ID, this->username))
                                    temp.push_back(temps);
                            }
                            temp.push_back(s);
                            tfile.close();
                            tfile.open(SHOPPINGFILE, ios::out | ios::binary);
                            for (vector<Shopping>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
                                temps = *iter;
                                tfile.write((char*)&temps, sizeof(Shopping));
                            }
                            tfile.close();
                            break;
                        }
                    }
                    iofile.close();
                    if (!isBreak) {
                        strcpy_s(s.cname, 20, e.name);
                        strcpy_s(s.ID, 20, this->username);
                        cout << "请输入添加数量：";
                        int addnum;
                        cin >> addnum;
                        while (addnum <= 0) {
                            cout << "添加数量需为正数，请重新输入：" << endl;
                            cin >> addnum;
                        }
                        while (addnum > e.surplus) {
                            cout << "添加数量需为正数，请重新输入：" << endl;
                            cin >> addnum;
                        }
                        s.number = addnum;
                        strcpy_s(s.type, 20, "elecgoods");
                    }
                    iofile.open(SHOPPINGFILE, ios::app | ios::binary);
                    if (!nosave)
                        iofile.write((char*)&s, sizeof(Shopping));
                    iofile.close();
                    nosave = 0;
                    break;
                }
            }
        }
        break;
    }
    default:
        break;
    }
}

void Consumer::viewShoppingCart() {//查看购物车
    Shopping s;
    int flag = 0;
    ifstream shoppingfile(SHOPPINGFILE, ios::in | ios::binary);
    while (shoppingfile.read((char*)&s, sizeof(Shopping))) {
        if (!strcmp(s.ID, this->username)) {
            flag = 1;
            cout << "名字：" << s.cname << " " << "数量：" << s.number << endl;
        }
    }
    if (!flag)
        cout << "购物车为空，请添加商品后查看！" << endl;
}

double Consumer::orderGeneration() {//生成订单
    Shopping s, temps;
    Consumer tempcsmer;
    Book b, tempb; Clothing c, tempc; Elecgoods e, tempe;
    double sumMoney = 0;
    int next = 0;
    int flag = 0;
    ifstream sfile(SHOPPINGFILE, ios::in | ios::binary);
    while (sfile.read((char*)&s, sizeof(Shopping))) {
        if (!strcmp(s.ID, this->username)) {
            ifstream infile(BOOKFILE, ios::in | ios::binary);
            while (infile.read((char*)&b, sizeof(Book))) {
                if (!strcmp(b.name, s.cname)) {
                    flag++;
                
                    b.surplus -= s.number;
                    sumMoney += b.price * s.number;
                  
                    next = 1;
                    //锁定商品
                    vector<Book> temp;
                    fstream iofile(BOOKFILE, ios::in | ios::binary);
                    while (iofile.read((char*)&tempb, sizeof(Book))) {
                        if (strcmp(tempb.name, b.name))
                            temp.push_back(tempb);
                    }
                    temp.push_back(b);
                    iofile.close();
                    iofile.open(BOOKFILE, ios::out | ios::binary);
                    for (vector<Book>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
                        tempb = *iter;
                        iofile.write((char*)&tempb, sizeof(Book));
                    }
                    iofile.close();
                    break;
                }
            }
            infile.close();
            if (next) {
                next = 0;//重置
                continue;
            }
            infile.open(CLOTHINGFILE, ios::in | ios::binary);
            while (infile.read((char*)&c, sizeof(Clothing))) {
                if (!strcmp(c.name, s.cname)) {
                    flag++;
            
                    c.surplus -= s.number;
                    sumMoney += c.price * s.number;
                   
                    next = 1;
                    //锁定商品
                    vector<Clothing> temp;
                    fstream iofile(CLOTHINGFILE, ios::in | ios::binary);
                    while (iofile.read((char*)&tempc, sizeof(Clothing))) {
                        if (strcmp(tempc.name, c.name))
                            temp.push_back(tempc);
                    }
                    temp.push_back(c);
                    iofile.close();
                    iofile.open(CLOTHINGFILE, ios::out | ios::binary);
                    for (vector<Clothing>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
                        tempc = *iter;
                        iofile.write((char*)&tempc, sizeof(Clothing));
                    }
                    iofile.close();

                    break;
                }
            }
            infile.close();
            if (next) {
                next = 0;//重置
                continue;
            }

            infile.open(ELECGOODSFILE, ios::in | ios::binary);
            while (infile.read((char*)&e, sizeof(Elecgoods))) {
                if (!strcmp(e.name, s.cname)) {
                    flag++;

                    e.surplus -= s.number;
                    sumMoney += e.price * s.number;
       
                    next = 1;
                    //锁定商品
                    vector<Elecgoods> temp;
                    fstream iofile(ELECGOODSFILE, ios::in | ios::binary);
                    while (iofile.read((char*)&tempe, sizeof(Elecgoods))) {
                        if (strcmp(tempe.name, e.name))
                            temp.push_back(tempe);
                    }
                    temp.push_back(e);
                    iofile.close();
                    iofile.open(ELECGOODSFILE, ios::out | ios::binary);
                    for (vector<Elecgoods>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
                        tempe = *iter;
                        iofile.write((char*)&tempe, sizeof(Elecgoods));
                    }
                    iofile.close();

                    break;
                }
            }
            infile.close();
            if (next) {
                next = 0;//重置
                continue;
            }
        }
    }
    sfile.close();

    this->sumMoney = sumMoney;//更新待支付金额

    //保存消费者的待支付金额信息
    vector<Consumer> tempcsm;
    fstream iofile(CONSUMERFILE, ios::in | ios::binary);
    while (iofile.read((char*)&tempcsmer, sizeof(Consumer))) {
        if (strcmp(tempcsmer.username, this->username))
            tempcsm.push_back(tempcsmer);
    }
    tempcsm.push_back(*this);
    iofile.close();
    iofile.open(CONSUMERFILE, ios::out | ios::binary);
    for (vector<Consumer>::iterator iter = tempcsm.begin(); iter != tempcsm.end(); ++iter) {
        tempcsmer = *iter;
        iofile.write((char*)&tempcsmer, sizeof(Consumer));
    }
    iofile.close();


    if (flag) {
        cout << "订单生成成功！" << endl;
        cout << "订单总金额为：" << sumMoney << endl;
    }
    else {
        sumMoney = 0;
        cout << "购物车中暂无商品，订单生成失败" << endl;
    }
    return sumMoney;
}

int Consumer::onlinePayment() {//网上支付
    if (this->balance < this->sumMoney) {
        cout << "支付失败！" << endl;
        cout << "账户余额不足，请充值后重试！" << endl;
        return 0;
    }
    if (!this->sumMoney) {
        cout << "待支付金额为零，无需支付！" << endl;
        return 0;
    }
    Consumer tempcsm;
    Business bsn,tempbsn;
    Shopping s, temps;
    Book b, tempb; Clothing c, tempc; Elecgoods e, tempe;
    double sumMoney = 0;
    int next = 0;
    int flag = 0;
    int maxnum;
    ifstream sfile(SHOPPINGFILE, ios::in | ios::binary);
    ifstream infile;
    while (sfile.read((char*)&s, sizeof(Shopping))) {
        if (!strcmp(s.ID, this->username)) {//如果这本书是这个消费者加购物车的
            infile.open(BOOKFILE, ios::in | ios::binary);
            while (infile.read((char*)&b, sizeof(Book))) {//查找对应商家
                if (!strcmp(b.name, s.cname)) {
                    flag++;
                    fstream checkfile;
                    checkfile.open(BUSINESSFILE, ios::in | ios::binary);
                    while (checkfile.read((char*)&bsn, sizeof(Business))) {
                        if (!strcmp(bsn.username, b.ID)) {
                            bsn.balance += b.price * s.number;
                            break;
                        }
                    }
                    checkfile.close();
                   
                    next = 1;
    
                    //删除现有购物车中信息
                    vector<Shopping> temp1;
                    fstream iofile(SHOPPINGFILE, ios::in | ios::binary);
                    while (iofile.read((char*)&temps, sizeof(Shopping))) {
                        if (strcmp(temps.cname, s.cname))
                            temp1.push_back(temps);
                    }
                    iofile.close();//删除时不需要再把更改后的加进去
                    iofile.open(SHOPPINGFILE, ios::out | ios::binary);
                    for (vector<Shopping>::iterator iter = temp1.begin(); iter != temp1.end(); ++iter) {
                        temps = *iter;
                        iofile.write((char*)&temps, sizeof(Shopping));
                    }
                    iofile.close();

                    //更新商家余额信息
                    vector<Business> temp2;
                    iofile.open(BUSINESSFILE, ios::in | ios::binary);
                    while (iofile.read((char*)&tempbsn, sizeof(Business))) {
                        if (strcmp(tempbsn.username, bsn.username))
                            temp2.push_back(tempbsn);
                    }
                    temp2.push_back(bsn);
                    iofile.close();
                    iofile.open(BUSINESSFILE, ios::out | ios::binary);
                    for (vector<Business>::iterator iter = temp2.begin(); iter != temp2.end(); ++iter) {
                        tempbsn = *iter;
                        iofile.write((char*)&tempbsn, sizeof(Business));
                    }
                    iofile.close();
                    
                    break;
                }
            }
            infile.close();
            if (next) {
                next = 0;//重置
                continue;
            }

            infile.open(CLOTHINGFILE, ios::in | ios::binary);
            while (infile.read((char*)&c, sizeof(Clothing))) {
                if (!strcmp(c.name, s.cname)) {
                    flag++;
                    fstream checkfile;

                    checkfile.open(BUSINESSFILE, ios::in | ios::binary);
                    while (checkfile.read((char*)&bsn, sizeof(Business))) {
                        if (!strcmp(bsn.username, c.ID)) {
                            bsn.balance += c.price * s.number;
                            break;
                        }
                    }
                    checkfile.close();
                  
                    next = 1;

                    //删除现有购物车中信息
                    vector<Shopping> temp1;
                    fstream iofile(SHOPPINGFILE, ios::in | ios::binary);
                    while (iofile.read((char*)&temps, sizeof(Shopping))) {
                        if (strcmp(temps.cname, s.cname))
                            temp1.push_back(temps);
                    }
                    iofile.close();//删除时不需要再把更改后的加进去
                    iofile.open(SHOPPINGFILE, ios::out | ios::binary);
                    for (vector<Shopping>::iterator iter = temp1.begin(); iter != temp1.end(); ++iter) {
                        temps = *iter;
                        iofile.write((char*)&temps, sizeof(Shopping));
                    }
                    iofile.close();

                    //更新商家余额信息
                    vector<Business> temp2;
                    iofile.open(BUSINESSFILE, ios::in | ios::binary);
                    while (iofile.read((char*)&tempbsn, sizeof(Business))) {
                        if (strcmp(tempbsn.username, bsn.username))
                            temp2.push_back(tempbsn);
                    }
                    temp2.push_back(bsn);
                    iofile.close();
                    iofile.open(BUSINESSFILE, ios::out | ios::binary);
                    for (vector<Business>::iterator iter = temp2.begin(); iter != temp2.end(); ++iter) {
                        tempbsn = *iter;
                        iofile.write((char*)&tempbsn, sizeof(Business));
                    }
                    iofile.close();

                    break;
                }
            }
            infile.close();
            if (next) {
                next = 0;//重置
                continue;
            }

            infile.open(ELECGOODSFILE, ios::in | ios::binary);
            while (infile.read((char*)&e, sizeof(Elecgoods))) {
                if (!strcmp(e.name, s.cname)) {
                    flag++;
                    fstream checkfile;

                    checkfile.open(BUSINESSFILE, ios::in | ios::binary);
                    while (checkfile.read((char*)&bsn, sizeof(Business))) {
                        if (!strcmp(bsn.username, c.ID)) {
                            bsn.balance += e.price * s.number;
                            break;
                        }
                    }
                    checkfile.close();
                 
                    next = 1;

                    //删除现有购物车中信息
                    vector<Shopping> temp1;
                    fstream iofile(SHOPPINGFILE, ios::in | ios::binary);
                    while (iofile.read((char*)&temps, sizeof(Shopping))) {
                        if (strcmp(temps.cname, s.cname))
                            temp1.push_back(temps);
                    }
                    iofile.close();//删除时不需要再把更改后的加进去
                    iofile.open(SHOPPINGFILE, ios::out | ios::binary);
                    for (vector<Shopping>::iterator iter = temp1.begin(); iter != temp1.end(); ++iter) {
                        temps = *iter;
                        iofile.write((char*)&temps, sizeof(Shopping));
                    }
                    iofile.close();

                    //更新商家余额信息
                    vector<Business> temp2;
                    iofile.open(BUSINESSFILE, ios::in | ios::binary);
                    while (iofile.read((char*)&tempbsn, sizeof(Business))) {
                        if (strcmp(tempbsn.username, bsn.username))
                            temp2.push_back(tempbsn);
                    }
                    temp2.push_back(bsn);
                    iofile.close();
                    iofile.open(BUSINESSFILE, ios::out | ios::binary);
                    for (vector<Business>::iterator iter = temp2.begin(); iter != temp2.end(); ++iter) {
                        tempbsn = *iter;
                        iofile.write((char*)&tempbsn, sizeof(Business));
                    }
                    iofile.close();

                    break;
                }
            }
            infile.close();
            if (next) {
                next = 0;//重置
                continue;
            }
        }

    }
    this->balance -= (this->sumMoney);//消费者的余额也要减掉
    //更新消费者余额信息
    vector<Consumer> temp3;
    fstream iofile(CONSUMERFILE, ios::in | ios::binary);
    while (iofile.read((char*)&tempcsm, sizeof(Consumer))) {
        if (strcmp(tempcsm.username, this->username))
            temp3.push_back(tempcsm);
    }
    temp3.push_back(*this);
    iofile.close();
    iofile.open(CONSUMERFILE, ios::out | ios::binary);
    for (vector<Consumer>::iterator iter = temp3.begin(); iter != temp3.end(); ++iter) {
        tempcsm = *iter;
        iofile.write((char*)&tempcsm, sizeof(Consumer));
    }
    iofile.close();
    cout << "支付成功" << endl;
    return 1;
}

int Consumer::coo() {//取消订单
    if (this->sumMoney == 0) {//说明并没有生成订单
        cout << "并无订单生成，无需取消！" << endl;
        return 0;
    }
    int have = 0;//标记是否有自己的商品
    int next = 0;
    Shopping s, temps;
    Book b, tempb; Clothing c, tempc; Elecgoods e, tempe;
    Consumer tempcsm;

    //先检测是否有自己的商品,若没有直接退出
    ifstream shoppingfile(SHOPPINGFILE, ios::in | ios::binary);
    while (shoppingfile.read((char*)&s, sizeof(Shopping))) {
        if (!strcmp(s.ID, this->username))
            have = 1;//表明有自己的商品
    }

    if (!have) {
        cout << "购物车为空，无新订单生成，无需取消订单！" << endl;
        return 0;
    }

    //在取消购物车订单之前，要先取消冻结的商品数量
    ifstream sfile(SHOPPINGFILE, ios::in | ios::binary);
    while (sfile.read((char*)&s, sizeof(Shopping))) {
        if (!strcmp(s.ID, this->username)) {//是该消费者的商品
            ifstream infile(BOOKFILE, ios::in | ios::binary);
            while (infile.read((char*)&b, sizeof(Book))) {
                if (!strcmp(b.name, s.cname)) {//若是图书
                    b.surplus += s.number;//返还冻结数量
                    next = 1;
                    //锁定商品
                    vector<Book> temp;
                    fstream iofile(BOOKFILE, ios::in | ios::binary);
                    while (iofile.read((char*)&tempb, sizeof(Book))) {
                        if (strcmp(tempb.name, b.name))
                            temp.push_back(tempb);
                    }
                    temp.push_back(b);
                    iofile.close();
                    iofile.open(BOOKFILE, ios::out | ios::binary);
                    for (vector<Book>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
                        tempb = *iter;
                        iofile.write((char*)&tempb, sizeof(Book));
                    }
                    iofile.close();
                    break;
                }
            }
            infile.close();
            if (next) {
                next = 0;//重置
                continue;
            }

            infile.open(CLOTHINGFILE, ios::in | ios::binary);
            while (infile.read((char*)&c, sizeof(Clothing))) {
                if (!strcmp(c.name, s.cname)) {//若是服装
                    c.surplus += s.number;//返还冻结数量
                    next = 1;
                    //锁定商品
                    vector<Clothing> temp;
                    fstream iofile(CLOTHINGFILE, ios::in | ios::binary);
                    while (iofile.read((char*)&tempc, sizeof(Clothing))) {
                        if (strcmp(tempc.name, c.name))
                            temp.push_back(tempc);
                    }
                    temp.push_back(c);
                    iofile.close();
                    iofile.open(CLOTHINGFILE, ios::out | ios::binary);
                    for (vector<Clothing>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
                        tempc = *iter;
                        iofile.write((char*)&tempc, sizeof(Clothing));
                    }
                    iofile.close();
                    break;
                }
            }
            infile.close();
            if (next) {
                next = 0;//重置
                continue;
            }
            infile.open(ELECGOODSFILE, ios::in | ios::binary);
            while (infile.read((char*)&e, sizeof(Elecgoods))) {
                if (!strcmp(e.name, s.cname)) {
                    e.surplus += s.number;//返还冻结数量
                    next = 1;
                    //锁定商品
                    vector<Elecgoods> temp;
                    fstream iofile(ELECGOODSFILE, ios::in | ios::binary);
                    while (iofile.read((char*)&tempe, sizeof(Elecgoods))) {
                        if (strcmp(tempe.name, e.name))
                            temp.push_back(tempe);
                    }
                    temp.push_back(e);
                    iofile.close();
                    iofile.open(ELECGOODSFILE, ios::out | ios::binary);
                    for (vector<Elecgoods>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
                        tempe = *iter;
                        iofile.write((char*)&tempe, sizeof(Elecgoods));
                    }
                    iofile.close();
                    break;
                }
            }
            infile.close();
            if (next) {
                next = 0;//重置
                continue;
            }
        }
    }

    //取消购物车中已添加的商品
    vector<Shopping> temp;
    shoppingfile.open(SHOPPINGFILE, ios::in | ios::binary);
    while (shoppingfile.read((char*)&s, sizeof(Shopping))) {
        
        if (strcmp(s.ID, this->username)) {//保存除自己商品之外的商品，即删除自己的商品
            if (s.number)//数量为0就不必再存进去
                temp.push_back(s);
        }
    }
    fstream iofile(SHOPPINGFILE, ios::out | ios::binary);
    for (vector<Shopping>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
        s = *iter;
        iofile.write((char*)&s, sizeof(Shopping));
    }
    iofile.close();

   
    this->sumMoney = 0;//同时还要修改待支付金额为零
     //保存消费者的待支付金额为零
   
    vector<Consumer> temp3;
    iofile.open(CONSUMERFILE, ios::in | ios::binary);
    while (iofile.read((char*)&tempcsm, sizeof(Consumer))) {
        if (strcmp(tempcsm.username, this->username))
            temp3.push_back(tempcsm);
    }
    temp3.push_back(*this);
    iofile.close();
    iofile.open(CONSUMERFILE, ios::out | ios::binary);
    for (vector<Consumer>::iterator iter = temp3.begin(); iter != temp3.end(); ++iter) {
        tempcsm = *iter;
        iofile.write((char*)&tempcsm, sizeof(Consumer));
    }
    iofile.close();

    return 1;    
}

int Consumer::rgstrCsm() {//注册消费者
    Consumer c;
    cout << "请输入消费者用户名：";
    cin >> this->username;
    fstream iofile(CONSUMERFILE, ios::in | ios::binary);
    if (!iofile) {
        cout << "Failed to open consumer file!" << endl;
        exit(0);
    }
    while (iofile.read((char*)&c, sizeof(Consumer))) {
        if (!strcmp(this->username, c.username)) {
            cout << "此用户名已存在，请重新输入：";
            cin >> this->username;
            iofile.seekg(0, ios::beg);
            continue;
        }
    }
    cout << "此用户名可用" << endl;
    cout << "请输入消费者密码(10位数字、字母或数字字母组合)：";
    cin >> this->password;
    while (strlen(this->password) != 10) {
        cout << "密码位数非10位，请重新输入：";
        cin >> this->password;
    }
    cout << "账户初始余额值为零" << endl;
    this->balance = 0;
    iofile.close();
    iofile.open(CONSUMERFILE, ios::app | ios::binary);
    iofile.write((char*)&(*this), sizeof(Consumer));
    cout << "注册成功！" << endl;
    iofile.close();
    return 2;
}

int Business::rgstrBsn() {//注册商家
    Business b;
    cout << "请输入商家用户名：";
    cin >> this->username;
    fstream iofile(BUSINESSFILE, ios::in | ios::binary);
    if (!iofile) {
        cout << "Failed to open business file!" << endl;
        exit(0);
    }
    while (iofile.read((char*)&b, sizeof(Business))) {
        if (!strcmp(b.username, this->username)) {
            cout << "此用户名已存在，请重新输入：";
            cin >> this->username;
            iofile.seekg(0, ios::beg);
            continue;
        }
    }
    cout << "此用户名可用" << endl;
    cout << "请输入商家密码(10位数字、字母或数字字母组合)：";
    cin >> this->password;
    while (strlen(this->password) != 10) {
        cout << "密码位数非10位，请重新输入：";
        cin >> this->password;
    }
    cout << "账户初始余额值为零" << endl;
    this->balance = 0;
    iofile.close();
    iofile.open(BUSINESSFILE, ios::app | ios::binary);
    iofile.write((char*)&(*this), sizeof(Business));
    cout << "注册成功！" << endl;
    iofile.close();
    return 1;
}

void Consumer::changeCsmPassword() {//修改消费者密码
    cout << "请输入新的消费者密码(10位数字、字母或数字字母组合)：";
    cin >> this->password;
    while (strlen(this->password) != 10) {
        cout << "新密码位数非10位，请重新输入：";
        cin >> this->password;
    }
    Consumer tempc;
    vector<Consumer> temp;
    fstream iofile(CONSUMERFILE, ios::in | ios::binary);
    while (iofile.read((char*)&tempc, sizeof(Consumer))) {  //暂存除要修改密码的其他用户
        if (strcmp(tempc.username, this->username))
            temp.push_back(tempc);
    }
    temp.push_back(*this);//把修改好密码的用户加进来
    iofile.close();
    iofile.open(CONSUMERFILE, ios::out | ios::binary);
    for (vector<Consumer>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
        tempc = *iter;
        iofile.write((char*)&tempc, sizeof(Consumer));
    }
    iofile.close();
}

void Business::changeBsnPassword() {//修改商家密码
    cout << "请输入新的商家密码(10位数字、字母或数字字母组合)：";
    cin >> password;
    while (strlen(password) != 10) {
        cout << "新密码位数非10位，请重新输入：";
        cin >> password;
    }
    Business tempb;
    vector<Business> temp;
    fstream iofile(BUSINESSFILE, ios::in | ios::binary);
    while (iofile.read((char*)&tempb, sizeof(Business))) {  
        if (strcmp(tempb.username, this->username))//暂存除要修改密码的其他用户
            temp.push_back(tempb);
    }
    temp.push_back(*this);//把修改好密码的用户加进来
    iofile.close();
    iofile.open(BUSINESSFILE, ios::out | ios::binary);
    for (vector<Business>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
        tempb = *iter;
        iofile.write((char*)&tempb, sizeof(Business));
    }
    iofile.close();
}

int Consumer::signInCsm() {
    int isBreak = 0;
    Consumer in;
    cout << "请输入消费者用户名：";
    cin >> this->username;
    ifstream infile;
    while (1) {
        infile.open(CONSUMERFILE, ios::in | ios::binary);
        while (infile.read((char*)&in, sizeof(in))) {
            if (!strcmp(in.username, this->username)) {
                cout << "请输入密码：" << endl;
                cin >> this->password;
                if (!strcmp(in.password, this->password)) {
                    cout << "登录成功！" << endl;
                    *this = in;
                    isBreak = 1;
                    break;
                }
            }
        }
        infile.close();
        if (isBreak) break;
        cout << "输入有误，请重新输入消费者用户名：" << endl;
        cin >> this->username;
    }
    return 2;
}

int Business::signInBsn() {
    Business in;
    int isBreak = 0;
    cout << "请输入商家用户名：";
    cin >> this->username;
    ifstream infile;
    while (1) {
        infile.open(BUSINESSFILE, ios::in | ios::binary);
        while (infile.read((char*)&in, sizeof(in))) {
            if (!strcmp(in.username, this->username)) {
                cout << "请输入密码：" << endl;
                cin >> this->password;
                if (!strcmp(in.password, this->password)) {
                    cout << "登录成功！" << endl;
                    *this = in;
                    isBreak = 1;
                    break;
                }
            }
        }
        infile.close();
        if (isBreak) break;
        cout << "输入有误，请重新输入商家用户名：" << endl;
        cin >> username;
    }
    return 1;
}

void Consumer::showCommodity() {//展示平台商品信息
    Book b;
    ifstream inBookFile(BOOKFILE, ios::in | ios::binary);
    if (!inBookFile)
        cout << "open bookfile failed!" << endl;
    while (inBookFile.read((char*)&b, sizeof(Book))) {
        cout << "书名：" << b.name << " " << "价格：" << b.getPrice() << " " << "作者：" << b.writer << " " << "剩余量：" << b.surplus << endl;
    }
    inBookFile.close();
    cout << endl;
    Clothing c;
    ifstream inClothingFile(CLOTHINGFILE, ios::in | ios::binary);
    if (!inClothingFile)
        cout << "open clothingfile failed!" << endl;
    while (inClothingFile.read((char*)&c, sizeof(Clothing))) {
        cout << "服装名：" << c.name << " " << "价格：" << c.getPrice() << " " << "原产国：" << c.from << " " << "剩余量：" << c.surplus << endl;
    }
    inClothingFile.close();
    cout << endl;
    Elecgoods e;
    ifstream inElecgoodsFile(ELECGOODSFILE, ios::in | ios::binary);
    if (!inElecgoodsFile)
        cout << "open elecgoodsfile failed!" << endl;
    while (inElecgoodsFile.read((char*)&e, sizeof(Elecgoods))) {
        cout << "电子产品名：" << e.name << " " << "价格：" << e.getPrice() << " " << "生产年份：" << e.yop << " " << "剩余量：" << e.surplus << endl;
    }
    inElecgoodsFile.close();
    cout << endl;
}

int Consumer::searchCommodity() {//搜索平台商品信息
    int result = 0;
    char searchName[40];
    cout << "请输入所查询的商品名称：";
    cin >> searchName;
    Book b;
    ifstream inBookFile(BOOKFILE, ios::in | ios::binary);
    if (!inBookFile) {
        cout << "open bookfile failed!" << endl;
        exit(0);
    }
    while (inBookFile.read((char*)&b, sizeof(Book))) {
        if (!strcmp(b.name, searchName)) {
            cout << "您所查询的商品信息如下：";
            cout << "价格：" << b.price << " " << "作者：" << b.writer << " " << "剩余量：" << b.surplus << endl;
            result = 1;
            break;
        }
    }
    inBookFile.close();
    if (result)
        return result;
    Clothing c;
    ifstream inClothingFile(CLOTHINGFILE, ios::in | ios::binary);
    if (!inClothingFile) {
        cout << "open clothingfile failed!" << endl;
        exit(0);
    }
    while (inClothingFile.read((char*)&c, sizeof(Clothing))) {
        if (!strcmp(c.name, searchName)) {
            cout << "您所查询的商品信息如下：";
            cout << "价格：" << c.price << " " << "原产国：" << c.from << " " << "剩余量：" << c.surplus << endl;
            result = 1;
            break;
        }
    }
    inClothingFile.close();
    if (result)
        return result;
    Elecgoods e;
    ifstream inElecgoodsFile(ELECGOODSFILE, ios::in | ios::binary);
    if (!inElecgoodsFile) {
        cout << "open elecgoodsfile failed!" << endl;
        exit(0);
    }
    while (inElecgoodsFile.read((char*)&e, sizeof(Elecgoods))) {
        if (!strcmp(e.name, searchName)) {
            cout << "您所查询的商品信息如下：";
            cout << "价格：" << e.price << " " << "生产年份：" << e.yop << " " << "剩余量：" << e.surplus << endl;
            result = 1;
            break;
        }
    }
    inElecgoodsFile.close();
    if (result)
        return result;
    return result;
}

int main() {
    cout << "                                                   电商平台                              " << endl;
    int select;
    int login = 0;//标记是否登录,1表示登录的是商家，2表示登陆的是消费者
    int sumMoney = 0;//订单总金额
    int paymentStatus;//支付状态
    int isCancel = 0;//订单取消状态
    int isFind = 0;//搜索平台信息的标志
    Business b;
    Consumer c;
    
    while (1) {
        cout << endl;
        cout << endl;
        //目录
        {
            cout << endl;
            cout << endl;
            cout << "                   ******************************1.用户登录***********************************" << endl;
            cout << endl;
            cout << "                   ******************************2.用户注册***********************************" << endl;
            cout << endl;
            cout << "                   ******************************3.密码修改***********************************" << endl;
            cout << endl;
            cout << "                   ******************************4.余额管理***********************************" << endl;
            cout << endl;
            cout << "                   ******************************5.添加商品***********************************" << endl;
            cout << endl;
            cout << "                   **************************6.展示平台商品信息*******************************" << endl;
            cout << endl;
            cout << "                   **************************7.搜索平台商品信息*******************************" << endl;
            cout << endl;
            cout << "                   ****************************8.商品信息管理*********************************" << endl;
            cout << endl;
            cout << "                   ****************************9.登陆状态查询*********************************" << endl;
            cout << endl;
            cout << "                   ****************************10.购物车管理**********************************" << endl;
            cout << endl;
            cout << "                   ****************************11.查看购物车**********************************" << endl;
            cout << endl;
            cout << "                   ****************************12.生成新订单**********************************" << endl;
            cout << endl;
            cout << "                   *****************************13.取消订单***********************************" << endl;
            cout << endl;
            cout << "                   *****************************14.网上支付***********************************" << endl;
            cout << endl;
            cout << endl; cout << endl; cout << endl; cout << endl; cout << endl;
        }
        cout << endl;
        cout << "商家文件中内容如下:" << endl;
        //展示商家
        {
            Business s;
            ifstream inFile(BUSINESSFILE, ios::in | ios::binary);
            if (!inFile) {
                cout << "商家文件中暂无内容，请添加后重试！" << endl;
                {
                    Business s;
                    int num;
                    cout << "暂无默认商家，请先添加默认商家：" << endl;
                    cout << "请输入添加商家个数：";
                    cin >> num;
                    ofstream outFile(BUSINESSFILE, ios::app | ios::binary);
                    if (!outFile) {
                        cout << "Failed to open business file!" << endl;
                        exit(0);
                    }
                    while (num) {
                        cout << "请输入商家用户名（6位数字）：";
                        cin >> s.username;
                        while (strlen(s.username) != 6) {
                            cout << "当前输入位数非6位，请重新输入：";
                            cin >> s.username;
                        }
                        cout << "请输入商家密码（10位数字、字母或数字字母组合）：";
                        cin >> s.password;
                        while (strlen(s.password) != 10) {
                            cout << "当前输入位数非10位，请重新输入：";
                            cin >> s.password;
                        }
                        cout << "请输入商家账户余额（5位数字）：";
                        cin >> s.balance;
                        outFile.write((char*)&s, sizeof(s));
                        num--;
                    }
                    outFile.close();
                }
            }
            while (inFile.read((char*)&s, sizeof(Business))) {
                cout <<"用户名："<< s.username << " " <<"密码："<< s.password << " " <<"余额："<< s.balance << " " << " " << s.type << endl;
            }
            inFile.close();
        }
        cout << endl;
        cout << "消费者文件中内容如下:" << endl;
        //展示消费者
        {
            Consumer s;
            ifstream inFile(CONSUMERFILE, ios::in | ios::binary);
            if (!inFile) {
                cout << "消费者文件中暂无内容，请添加后重试！" << endl;
                {
                    Consumer c;
                    int num;
                    cout << "暂无默认消费者，请先添加默认消费者：" << endl;
                    cout << "请输入添加消费者个数：";
                    cin >> num;
                    ofstream outFile(CONSUMERFILE, ios::app | ios::binary);
                    if (!outFile) {
                        cout << "Failed to open consumer file!" << endl;
                        exit(0);
                    }
                    while (num) {
                        cout << "请输入消费者用户名（6位数字）：";
                        cin >> c.username;
                        while (strlen(c.username) != 6) {
                            cout << "当前输入位数非6位，请重新输入：";
                            cin >> c.username;
                        }
                        cout << "请输入消费者密码（10位数字、字母或数字字母组合）：";
                        cin >> c.password;
                        while (strlen(c.password) != 10) {
                            cout << "当前输入位数非10位，请重新输入：";
                            cin >> c.password;
                        }
                        cout << "请输入消费者账户余额（5位数字）：";
                        cin >> c.balance;
                        outFile.write((char*)&c, sizeof(Consumer));
                        num--;
                    }
                    outFile.close();
                }
            }
            while (inFile.read((char*)&s, sizeof(Consumer))) {
                cout << "用户名：" << s.username << " " << "密码：" << s.password << " " << "余额：" << s.balance << " " << " " << s.type << endl;
            }
            inFile.close();
        }
        cout << endl;
        cout << "请输入您想要进行的操作编号：";
        cin >> select;
        while (select < 1 || select > 14) {
            cout << "编号输入有误，请重新输入：";
            cin >> select;
        }
        cout << endl;
        switch (select) {
        case 1:
            int select;
            cout << "请选择登录的用户类型：";
            cout << "    1.商家             2.消费者" << endl;
            cin >> select;
            while (select != 1 && select != 2) {
                cout << "编号输入有误，请重新输入：";
                cin >> select;
            }
            if (select == 1)
                login = b.signInBsn();
            else
                login = c.signInCsm();
            break;
        case 2:
            cout << "请选择注册的用户类型：";
            cout << "    1.商家             2.消费者" << endl;
            cin >> select;
            while (select != 1 && select != 2) {
                cout << "编号输入有误，请重新输入：";
                cin >> select;
            }
            if (select == 1)
                login = b.rgstrBsn();
            else
                login = c.rgstrCsm();
            cout << endl;
            cout << "您所注册账户已成功登陆！" << endl;
            break;
        case 3:
            if (login == 1)
                b.changeBsnPassword();
            else if (login == 2)
                c.changeCsmPassword();
            else
                cout << "您尚未登录，无法进行密码修改！" << endl;
            break;
        case 4:
            if (login == 1)
                b.balanceChange();
            else if (login == 2)
                c.balanceChange();
            else
                cout << "您尚未登录，无法进行余额管理！" << endl;
            break;
        case 5:
        {
            if (login == 1) {
                int select = 0;
                cout << "请选择想要添加的商品类型（编号）：" << endl;
                cout << "1.图书               2.服装               3.电子产品" << endl;
                cin >> select;
                while (select != 1 && select != 2 && select != 3) {
                    cin >> select;
                }
                switch (select) {
                case 1:
                    b.addbook();
                    break;
                case 2:
                    b.addclothes();
                    break;
                case 3:
                    b.addelecgoods();
                    break;
                default:
                    break;
                }
            }
            else if (login == 2)
                cout << "很抱歉，消费者无法添加商品！" << endl;
            else
                cout << "尚未登录，无法进行该操作！" << endl;
            break;
        }
        case 6:
            c.showCommodity();
            break;
        case 7:
            isFind = c.searchCommodity();
            if (!isFind)
                cout << "您说输入的信息未查询到！" << endl;
            break;
        case 8:
            if (login == 1)
                b.cim();
            else if (login == 2)
                cout << "很抱歉，消费者无法管理商品！" << endl;
            else
                cout << "尚未登录，无法进行该操作！" << endl;
            break;
        case 9:
            if (login == 1) {
                cout << "您当前所登录的账户类型为：" << b.getUserType() << endl;
                cout << "用户名：" << b.username << " " << "密码：" << b.password << " " << "账户余额：" << b.balance << endl;
            }

            else if (login == 2) {
                cout << "您当前所登录的账户类型为：" << c.getUserType() << endl;
                cout << "用户名：" << c.username << " " << "密码：" << c.password << " " << "账户余额：" << c.balance << endl;
            }

            else
                cout << "您当前尚未登录" << endl;
            break;
        case 10:
            if (login == 2)
                c.ShoppingCartChange();
            else {
                cout << "很抱歉，您不是消费者，不能对购物车进行管理！" << endl;
            }
            break;
        case 11:
            if (login == 2)
                c.viewShoppingCart();
            else if (login == 1)
                cout << "很抱歉，商家无法查看购物车！" << endl;
            else
                cout << "尚未登录，无法进行该操作！" << endl;
            break;
        case 12:
            if (login == 2)
                c.sumMoney = c.orderGeneration();
            else if (login == 1)
                cout << "商家无法生成新订单" << endl;
            else
                cout << "尚未登录，无法进行该操作！" << endl;
            break;
        case 13:
            if (login == 2) {
                isCancel = paymentStatus = c.coo();
                if (isCancel)
                    cout << "订单已成功取消！" << endl;
            }
            else if (login == 1)
                cout << "商家无法取消购物车订单" << endl;
            else
                cout << "尚未登录，无法进行该操作！" << endl;
            break;
        case 14:
            if (login == 2)
                paymentStatus = c.onlinePayment();
            else if (login == 1)
                cout << "商家无法进行网上支付" << endl;
            else
                cout << "尚未登录，无法进行该操作！" << endl;
            break;
        default:
            break;
        }
    }
    return 0;
}
