#include <iostream>
#include <fstream>
#include <vector>
#include <random>

using namespace std;

struct Pixel {                
    unsigned char b, g, r;
};

int main() {
    ifstream fin("C:/Users/Raodgan/Desktop/BMP_Lab10/1.bmp", ios::binary);

    fin.seekg(0, ios::end);
    size_t fileSize = fin.tellg();
    fin.seekg(0);

    vector<unsigned char> original(fileSize);
    fin.read(reinterpret_cast<char*>(original.data()), fileSize);
    fin.close();

    auto u32 = [&](size_t pos) {
        return *reinterpret_cast<uint32_t*>(&original[pos]);
        };
    uint32_t offset = u32(10); 
    uint32_t width = u32(18);
    uint32_t height = u32(22);

    size_t pixels = width * height;
    Pixel* img = reinterpret_cast<Pixel*>(&original[offset]);

    auto neg = original;
    auto halfRB = original;
    auto noisy = original;

    Pixel* pNeg = reinterpret_cast<Pixel*>(&neg[offset]);
    Pixel* pHalfRB = reinterpret_cast<Pixel*>(&halfRB[offset]);
    Pixel* pNoise = reinterpret_cast<Pixel*>(&noisy[offset]);


    mt19937 gen(random_device{}());
    for (size_t i = 0; i < pixels; ++i) {
        pNeg[i].r = 255 - pNeg[i].r;
        pNeg[i].g = 255 - pNeg[i].g;
        pNeg[i].b = 255 - pNeg[i].b;

        pHalfRB[i].r = static_cast<unsigned char>(pHalfRB[i].r / 2);
        pHalfRB[i].b = static_cast<unsigned char>(pHalfRB[i].b / 2);

        uniform_int_distribution<int> dR(0, pNoise[i].r / 2);
        uniform_int_distribution<int> dG(0, pNoise[i].g / 2);
        uniform_int_distribution<int> dB(0, pNoise[i].b / 2);

        int addR = dR(gen), addG = dG(gen), addB = dB(gen);
        pNoise[i].r = static_cast<unsigned char>(min(255, pNoise[i].r + addR));
        pNoise[i].g = static_cast<unsigned char>(min(255, pNoise[i].g + addG));
        pNoise[i].b = static_cast<unsigned char>(min(255, pNoise[i].b + addB));
    }

    auto save = [&](const string& name, const vector<unsigned char>& buf) {
        ofstream fout(name, ios::binary);
        fout.write(reinterpret_cast<const char*>(buf.data()), buf.size());
        };
    save("C:\\Users\\Raodgan\\Desktop\\BMP_Lab10\\2.bmp", neg);
    save("C:\\Users\\Raodgan\\Desktop\\BMP_Lab10\\3.bmp", halfRB);
    save("C:\\Users\\Raodgan\\Desktop\\BMP_Lab10\\4.bmp", noisy);
}
