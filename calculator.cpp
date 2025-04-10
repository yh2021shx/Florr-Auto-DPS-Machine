#include <bits/stdc++.h>
using namespace std;

const long double pi = acos(-1);
const long double eps = 0.30;

ifstream Talent, Petals, Names;
long double speed, lucky, cards, ReloadTalent, armors, MobsDamage, DUP, MobsHealth;
string PetalsName[207];
map <string, int> NameMap;
int UserPetals[27][27];
string UserLevels[27][27];
int Triangle, T;
ofstream out;

struct APetal {
    int id, damage, health;
    long double reload;
    int num;
} tx[17];

void ReadTalent() {
    Talent.open("./UserInfo/Talent.txt", ios::in);
    string tmp;
    Talent >> tmp;
    Talent >> tmp >> speed;
    Talent >> tmp >> lucky;
    Talent >> tmp >> cards;
    Talent >> tmp >> DUP;
    Talent >> tmp >> ReloadTalent;
    Talent >> tmp >> armors;
    Talent >> tmp >> MobsDamage;
    Talent >> tmp >> MobsHealth;
    Talent.close();
}

void ReadPetals() {
    Petals.open("./UserInfo/Petals.txt", ios::in);
    string tmp;
    getline(Petals, tmp);
    Petals >> T;
    for (int j = 1; j <= T; j ++) {
        for (int i = 1; i <= cards; i ++) {
            Petals >> tmp;
            UserPetals[j][i] = NameMap[tmp];
            Triangle += (UserPetals[j][i] == 107);
            cout << UserPetals[j][i] << ' ';
        }
        cout << '\n';
        for (int i = 1; i <= cards; i ++) {
            Petals >> UserLevels[j][i];
        }
    }
}

void LoadPetalsName() {
    Names.open("petals_names.txt", ios::in);
    string tmp; int id = 0;
    while (Names >> tmp) {
        PetalsName[++ id] = tmp;
        NameMap[tmp] = id;
    }
}

APetal GetPetalInfo(int id, int pid, int wjc) {
    string FileName = "./inf/Petal_" + to_string(id) + "_Record.txt";
    ifstream in; in.open(FileName.c_str(), ios::in);
    string tmp; in >> tmp >> tmp;
    APetal res = {id, 0, 0, 0};
    for (int i = 1; i <= 9; i ++) {
        in >> tmp; tmp.pop_back();
        in >> res.damage >> res.health >> res.reload >> res.num;
        if (tmp == UserLevels[wjc][pid]) {
            in.close();
            // cout << PetalsName[id] << ": " << res.damage << ' ' << res.health << ' ' << res.reload << '\n';
            return res;
        }
    }
    in.close();
    out << "未找到花瓣：" << id << '\n';
    return res;
}

long double SumDamage;

void DealInfo(APetal &p, int pid, int wjc) {
    if (p.id == 12) {
        if (UserLevels[wjc][pid] == "普通") speed += 0.5;
        if (UserLevels[wjc][pid] == "罕见") speed += 0.7;
        if (UserLevels[wjc][pid] == "稀有") speed += 0.9;
        if (UserLevels[wjc][pid] == "史诗") speed += 1.1;
        if (UserLevels[wjc][pid] == "传奇") speed += 1.3;
        if (UserLevels[wjc][pid] == "神话") speed += 1.5;
        if (UserLevels[wjc][pid] == "究极") speed += 1.7;
        if (UserLevels[wjc][pid] == "超级") speed += 1.9;
        if (UserLevels[wjc][pid] == "唯一") speed += 2.1;
    } else if (p.id == 90) {
        // 珊瑚 特殊机制：致敬传奇分裂王
        tx[pid].damage *= 3;
    } else if (p.num > 1) {
        // 天赋 重瓣
        p.num += DUP;
    } else if (p.id == 64) {
        // 刺果 打破护甲
        armors = 0;
    } else if (p.id == 107) {
        // 三角形 叠加
        if (UserLevels[wjc][pid] == "普通") p.damage += (Triangle - 1) * 4;
        if (UserLevels[wjc][pid] == "罕见") p.damage += (Triangle - 1) * 12;
        if (UserLevels[wjc][pid] == "稀有") p.damage += (Triangle - 1) * 36;
        if (UserLevels[wjc][pid] == "史诗") p.damage += (Triangle - 1) * 108;
        if (UserLevels[wjc][pid] == "传奇") p.damage += (Triangle - 1) * 324;
        if (UserLevels[wjc][pid] == "神话") p.damage += (Triangle - 1) * 972;
        if (UserLevels[wjc][pid] == "究极") p.damage += (Triangle - 1) * 2916;
        if (UserLevels[wjc][pid] == "超级") p.damage += (Triangle - 1) * 8748;
        if (UserLevels[wjc][pid] == "唯一") p.damage += (Triangle - 1) * 26244;
    }
}

void CalculatePetal(APetal p, int pid, int wjc) {
    long double sum = 0, tim = 0;
    long double angle = 2.0 * pi / cards * (pid - 1);
    for (int i = 1; i <= 2000; i ++) {
        if (fabs(angle) > eps && fabs(2 * pi - angle) > eps) {
            tim += (2.0 * pi - eps - angle) / speed;
            angle = 2.0 * pi - eps;
        }
        long double D = (p.damage - armors) * p.num;
        if (p.id == 90) {
            // 珊瑚 armors 翻六倍
            D -= armors * 5;
        }
        // printf("%.10Lf\n", D * ceil(p.health / MobsDamage));
        sum += max((long double) 0, D * ceil(p.health / MobsDamage));
        tim += p.reload * (1 - ReloadTalent);
        angle += p.reload * (1 - ReloadTalent) * speed;
        angle = angle - floor(angle / 2.0 / pi) * 2.0 * pi;
    }
    SumDamage += sum / tim;
    out << UserLevels[wjc][pid] << PetalsName[p.id] << ": ";
    out << fixed << setprecision(10) << sum / tim << '\n';
}

signed main() {
    LoadPetalsName();
    ReadTalent();
    ReadPetals();
    for (int j = 1; j <= T; j ++) {
        string strstr = "./Result/Build_" + to_string(j) + ".txt";
        out.open(strstr.c_str(), ios::out);
        ReadTalent();
        SumDamage = 0;
        for (int i = 1; i <= cards; i ++) {
            tx[i] = GetPetalInfo(UserPetals[j][i], i, j);
        }
        for (int i = 1; i <= cards; i ++)
            DealInfo(tx[i], i, j);
        out << "----------------------\n";
        for (int i = 1; i <= cards; i ++)
            CalculatePetal(tx[i], i, j);
        out << "----------------------\n";
        out << "DPS : " << fixed << setprecision(10) << SumDamage << '\n';
        out << "消灭生物所需时间： " << fixed << setprecision(10) << MobsHealth / SumDamage << '\n';
        out << "抢掉落物所需时间： " << fixed << setprecision(10) << MobsHealth / SumDamage / 4.0 << '\n';
        out.close();
    }
    return 0;
}

/*
Build #1:
珊瑚 珊瑚 沙子 沙子 刺果 海星 硬币 更快 更快 海星
神话 神话 神话 神话 神话 神话 究极 神话 神话 神话
Build #2:
轻   轻  轻   轻  更快 更快 硬币 海星 海星 刺果
神话 神话 神话 神话 神话 神话 究极 神话 神话 神话

*/
