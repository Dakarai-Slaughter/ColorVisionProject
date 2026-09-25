#include <iostream>
#include <cmath>
using namespace std;

int main()
{
  int red_1;
  int green_1;
  int blue_1;
  int red_2;
  int green_2;
  int blue_2;
  
while (true){
  cout<<"Please enter the RGB value of the first color sepperated by spaces."<<endl;
  cin>>red_1;
  cin>>green_1;
  cin>>blue_1;
  cout<<"Please enter the RGB value of the second color sepperated by spaces."<<endl;
  cin>>red_2;
  cin>>green_2;
  cin>>blue_2;
  cout<<red_1<<" "<<green_1<<" "<<blue_1<<" "<<red_2<<" "<<green_2<<" "<<blue_2<<endl;

  cout<<"The differance score is "<<(abs(red_1-red_2)+abs(green_1-green_2)+abs(blue_1-blue_2))<<endl;
}
  return 0;
}
