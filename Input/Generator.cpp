#include <cstdio>
#include <cstdlib>
#include <time.h>
using namespace std;
bool x[1005][1005];
int main(){
    int n,t;
    srand(time(NULL));
    printf("Input 1 to 1000:");
    freopen("out.txt","w",stdout);
    scanf("%d",&n);
    for(int i = 0 ;i < n ;i++){
        for(int j = 0 ;j < n ;j++){
            x[i][j]=rand()%2;
        }
    }
    printf("$1\n");
    /*for(int i = 0 ;i < n ;i++){
        for(int j = 0 ;j < n ;j++){
            printf("%d",x[i][j]);
        }
        puts("");
    }*/
    for(int i = 0 ;i < n ; i++){
        int now = 0;
        for(int j = 0 ;j < n ;j++){
            if(x[i][j]) now ++;
            else if(now !=0 ){
                printf("%d  ",now);
                now = 0;
            }
        }
        if(now !=0 ){
            printf("%d",now);
            now = 0;
        }
        printf("\n");
    }
    for(int i = 0 ;i < n ; i++){
        int now = 0;
        for(int j = 0 ;j < n ;j++){
            if(x[j][i]) now ++;
            else if(now !=0 ){
                printf("%d  ",now);
                now = 0;
            }
        }
        if(now !=0 ){
            printf("%d",now);
            now = 0;
        }
        printf("\n");
    }
}
