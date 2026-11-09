#include<iostream>
#include<string>
#include<cmath>
#include<vector>
#include<fstream>
#include<algorithm>
#include<sstream>
#include<cctype>
using namespace std;

struct COLOR {
    // RGB values
    double r = 0;
    double g = 0;
    double b = 0;
    // Linear RGB values
    double Lr = 0;
    double Lg = 0;
    double Lb = 0;
    // XYZ values
    double X = 0;
    double Y = 0;
    double Z = 0;
    // adjusted XYZ values
    double Ax = 0;
    double Ay = 0;
    double Az = 0;
    // Lab values
    double L = 0;
    double A = 0;
    double B = 0;

    int ID = 0;
};

bool isValidHex(string color) {
    if (color.length() != 7 || color[0] != '#') {
        return false;
    }
    for (int i = 1; i < 7; i++) {
        if (!isxdigit(color[i])) {
            return false;
        }
    }

    return true;
}

bool isValidRGB(double red, double green, double blue) {
    if (red < 0 || red > 255 || green < 0 || green > 255 || blue < 0 || blue > 255) {
        return false;
    }
    return true;
}

COLOR hexToRGB(string hex) {
    COLOR color;
    color.r = stoi(hex.substr(1, 2), nullptr, 16);
    color.g = stoi(hex.substr(3, 2), nullptr, 16);
    color.b = stoi(hex.substr(5, 2), nullptr, 16);
    return color;
}

COLOR cleanString(string input) {
    if (isValidHex(input)) {
        COLOR color = hexToRGB(input);
        return color;
    }else {
        COLOR color;
        replace(input.begin(), input.end(), ',', ' ');
        istringstream iss(input);
        if (!(iss >> color.r >> color.g >> color.b)){
            return COLOR{-1,-1,-1}; // Invalid color
        }
        if (isValidRGB(color.r, color.g, color.b)) {
            return color;
        }
    }
    return COLOR{ -1, -1, -1 }; // Invalid color
}

vector<COLOR> getInputCIN() {
    vector<COLOR> colors;
    string input;
    cout << "Enter as many colors as you would like and hit enter after each color. Type 'done' to finish." << endl;
        int i = 1;
    while (true) {
        cout << "Enter color: ";
        getline(cin, input);
        if (input == "done") {
            break;
        }
        COLOR color = cleanString(input);
        if (color.r != -1) {
            color.ID = i;
            colors.push_back(color);
            i++;
        } else {
            cout << "Invalid color format. Please try again." << endl;
        }
    }
    return colors;
}

vector<COLOR> getInputFile() {
    vector<COLOR> colors;
    string filename;
    cout << "Please make sure that each color is on a new line in the file." << endl;
    cout << "Enter the filename containing colors: ";
    cin >> filename;
    ifstream file(filename);
    if (!file) {
        cout << "Error opening file." << endl;
        return colors;
    }
    string line;
    int i = 1;
    while (getline(file, line)) {
        COLOR color = cleanString(line);
        if (color.r != -1) {
            color.ID = i;
            colors.push_back(color);
            i++;
        } else {
            cout << "Invalid color format in file: " << line << endl;
        }
    }
    return colors;
}

void rescaleColors(COLOR & color) {
    COLOR rescaled;
    rescaled.r = color.r / 255.0;
    rescaled.g = color.g / 255.0;
    rescaled.b = color.b / 255.0;
    color.r = rescaled.r;
    color.g = rescaled.g;
    color.b = rescaled.b;
}

void sRGBtoLinearRGB(COLOR & color) {
    color.Lr = (color.r <= 0.04045) ? (color.r) / 12.92 : pow((color.r+ 0.055) / 1.055, 2.4);
    color.Lg = (color.g <= 0.04045) ? (color.g) / 12.92 : pow((color.g+ 0.055) / 1.055, 2.4);
    color.Lb = (color.b <= 0.04045) ? (color.b) / 12.92 : pow((color.b+ 0.055) / 1.055, 2.4);
}

void linearRGBtoXYZ(COLOR & color) {
    color.X = color.Lr * 0.4124564 + color.Lg * 0.3575761 + color.Lb * 0.1804375;
    color.Y = color.Lr * 0.2126729 + color.Lg * 0.7151522 + color.Lb * 0.0721750;
    color.Z = color.Lr * 0.0193339 + color.Lg * 0.1191920 + color.Lb * 0.9503041;

}

void adjustXYZ(COLOR & color) {

    color.Ax = color.X / 0.95047;
    color.Ay = color.Y / 1;
    color.Az = color.Z / 1.08883;
    color.Ax = (color.Ax > (6.0/29.0) * (6.0/29.0) * (6.0/29.0)) ? cbrt(color.Ax) : color.Ax / (3 * (6.0/29.0) * (6.0/29.0)) + 4.0 / 29.0;
    color.Ay = (color.Ay > (6.0/29.0) * (6.0/29.0) * (6.0/29.0)) ? cbrt(color.Ay) : color.Ay / (3 * (6.0/29.0) * (6.0/29.0)) + 4.0 / 29.0;
    color.Az = (color.Az > (6.0/29.0) * (6.0/29.0) * (6.0/29.0)) ? cbrt(color.Az) : color.Az / (3 * (6.0/29.0) * (6.0/29.0)) + 4.0 / 29.0;
}

void xyztoLab(COLOR & color) {
    color.L = 116 * color.Ay - 16;
    color.A = 500 * (color.Ax - color.Ay);
    color.B = 200 * (color.Ay - color.Az);
}

void simulateVision(COLOR & color, char choice) {
    double r = color.Lr;
    double g = color.Lg;
    double b = color.Lb;
    double newr = r;
    double newg = g;
    double newb = b;

    if (choice == '1') {          // protanopia
        newr =  0.152286 * r + 1.052583 * g - 0.204868 * b;
        newg =  0.114503 * r + 0.786281 * g + 0.099216 * b;
        newb = -0.003882 * r - 0.048116 * g + 1.051998 * b;
    } else if (choice == '2') {   // deuteranopia
        newr =  0.367322 * r + 0.860646 * g - 0.227968 * b;
        newg =  0.280085 * r + 0.672501 * g + 0.047413 * b;
        newb = -0.011820 * r + 0.042940 * g + 0.968881 * b;
    } else if (choice == '3') {   // tritanopia
        newr =  1.255528 * r - 0.076749 * g - 0.178779 * b;
        newg = -0.078411 * r + 0.930809 * g + 0.147602 * b;
        newb =  0.004733 * r + 0.691367 * g + 0.303900 * b;
    }

    color.Lr = max(0.0, min(1.0, newr));
    color.Lg = max(0.0, min(1.0, newg));
    color.Lb = max(0.0, min(1.0, newb));
}

//NOT MY CODE, I DID NOT WRITE THIS FUNCTION. IT IS AN ESTABLISHED FUNCTION FOR CALCULATING COLOR DIFFERENCE USING THE CIEDE2000 FORMULA.
double deltaE2000(const COLOR& c1, const COLOR& c2) {
    const double PI = 3.14159265358979323846;
    auto rad = [&](double d) { return d * PI / 180.0; };
    auto deg = [&](double r) { return r * 180.0 / PI; };

    double L1 = c1.L, a1 = c1.A, b1 = c1.B;
    double L2 = c2.L, a2 = c2.A, b2 = c2.B;

    double C1 = hypot(a1, b1);
    double C2 = hypot(a2, b2);
    double Cbar = (C1 + C2) / 2.0;

    double Cbar7 = pow(Cbar, 7);
    double G = 0.5 * (1.0 - sqrt(Cbar7 / (Cbar7 + pow(25.0, 7))));

    double a1p = (1.0 + G) * a1;
    double a2p = (1.0 + G) * a2;

    double C1p = hypot(a1p, b1);
    double C2p = hypot(a2p, b2);

    auto hue = [&](double b, double ap) {
        if (b == 0.0 && ap == 0.0) return 0.0;
        double h = deg(atan2(b, ap));
        return (h < 0) ? h + 360.0 : h;
    };
    double h1p = hue(b1, a1p);
    double h2p = hue(b2, a2p);

    double dLp = L2 - L1;
    double dCp = C2p - C1p;

    double dhp = 0.0;
    if (C1p * C2p != 0.0) {
        dhp = h2p - h1p;
        if (dhp > 180.0)       dhp -= 360.0;
        else if (dhp < -180.0) dhp += 360.0;
    }
    double dHp = 2.0 * sqrt(C1p * C2p) * sin(rad(dhp / 2.0));

    double Lbarp = (L1 + L2) / 2.0;
    double Cbarp = (C1p + C2p) / 2.0;

    double hbarp;
    if (C1p * C2p == 0.0) {
        hbarp = h1p + h2p;
    } else if (fabs(h1p - h2p) <= 180.0) {
        hbarp = (h1p + h2p) / 2.0;
    } else if (h1p + h2p < 360.0) {
        hbarp = (h1p + h2p + 360.0) / 2.0;
    } else {
        hbarp = (h1p + h2p - 360.0) / 2.0;
    }

    double T = 1.0
             - 0.17 * cos(rad(hbarp - 30.0))
             + 0.24 * cos(rad(2.0 * hbarp))
             + 0.32 * cos(rad(3.0 * hbarp + 6.0))
             - 0.20 * cos(rad(4.0 * hbarp - 63.0));

    double dTheta = 30.0 * exp(-pow((hbarp - 275.0) / 25.0, 2));
    double Cbarp7 = pow(Cbarp, 7);
    double Rc = 2.0 * sqrt(Cbarp7 / (Cbarp7 + pow(25.0, 7)));

    double Lm50sq = (Lbarp - 50.0) * (Lbarp - 50.0);
    double Sl = 1.0 + (0.015 * Lm50sq) / sqrt(20.0 + Lm50sq);
    double Sc = 1.0 + 0.045 * Cbarp;
    double Sh = 1.0 + 0.015 * Cbarp * T;
    double Rt = -sin(rad(2.0 * dTheta)) * Rc;

    double tl = dLp / Sl;
    double tc = dCp / Sc;
    double th = dHp / Sh;

    return sqrt(tl * tl + tc * tc + th * th + Rt * tc * th);
}



int main() {
    // declare variables
    vector<COLOR> colors;
    
    // intro to the color program and instructions for user
    cout << "Welcome to the Color Difference Program!" << endl;
    cout << "You can enter colors in hex format (e.g., #FF5733) or RGB format (e.g., 255,87,51)." << endl;
    cout << "Would you like to input colors directly or read them from a file?" << endl;
    while (true) {
        cout << "Enter '1' for direct input or '2' for file input: ";
        string choice;
        cin >> choice;
        cin.ignore(); // Clear the newline character from the input buffer
        if (choice == "1") {
            colors = getInputCIN();
            if (colors.empty()) {
                cout << "No valid colors were given. Please try again." << endl;
            } else if (colors.size() < 2) {
                cout << "At least two colors are required for comparison." << endl;
            } else {
                break;
            }
        } else if (choice == "2") {
            colors = getInputFile();
            if (colors.empty()) {
                cout << "No valid colors were read from the file. Please try again." << endl;
            } else if (colors.size() < 2) {
                cout << "At least two colors are required for comparison." << endl;
            } else {
                break;
            }
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    for (COLOR & color : colors) {
        rescaleColors(color);
        sRGBtoLinearRGB(color);
    }

    string choice;
    while (true) {
        cout << "what kind of color blindness would you like to simulate? (Enter '0' for no simulation, '1' for protanopia, '2' for deuteranopia, or '3' for tritanopia): ";
        cin >> choice;
        if(choice.length() != 1 || (choice[0] < '0' || choice[0] > '3')) {
            cout << "Invalid choice. Please try again." << endl;
            continue;
        }else {
            cin.ignore();
            break;
        }
    }

        if (choice == "0"){
            cout << "No color blindness simulation selected." << endl;
        }else if (choice == "1") {
            cout << "Protanopia simulation selected." << endl;
            for (COLOR & color : colors) {
                simulateVision(color, choice[0]);
            }
        }else if (choice == "2") {
            cout << "Deuteranopia simulation selected." << endl;
            for (COLOR & color : colors) {
                simulateVision(color, choice[0]);
            }
        }else if (choice == "3") {
            cout << "Tritanopia simulation selected." << endl;
            for (COLOR & color : colors) {
                simulateVision(color, choice[0]);
            }
        }



    for (COLOR & color : colors) {
        linearRGBtoXYZ(color);
        adjustXYZ(color);
        xyztoLab(color);
    }


    for (const COLOR & color1 : colors) {
        for (const COLOR & color2 : colors) {
            if (&color1 != &color2) { // Avoid comparing the same color
                double difference = deltaE2000(color1, color2);
                cout << "Delta E 2000 of color "<< color1.ID<< " and " << color2.ID <<" is: " << difference << endl;
                if (difference < 1) {
                    cout << "The colors look identical." << endl;
                } else if (difference < 2) {
                    cout << "The colors look similar." << endl;
                } else if (difference < 10) {
                    cout << "The colors look different." << endl;
                } else if (difference < 50) {
                    cout << "The colors look clearly different." << endl;
                }else {
                    cout << "The colors look extremely different." << endl;
                }
            }
        }

    }
    return 0;
}