#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N_MAX 100

int input_a (long int *); //平文の入力
long int gcd (long int ,long int ); //最大公約数
void key_gen(long int *,int ,long int *,long int *,long int *); //鍵生成
void print_array(long int *,int); //配列出力
void encryption(long int *,long int *,long int *,int );//暗号化
long int inverse(long int ,long int );//逆元
void decryption(long int ,long int ,long int ,long int *,long int *,int ,long int );//復号
void fprint_array(long int *,int );  //ファイル出力用

int main() {
	long int w[N_MAX],a[N_MAX],pub[N_MAX],de_a[N_MAX];
	long int sum,inv,q,r,c;
	int i,bit;

// init
	FILE *out;
	out = fopen("out.txt","w");
	fprintf(out,"");
	fclose(out);
	c = bit = i = q = r = sum = inv = 0;
	for (i = 0;i < N_MAX;i ++) w[i] = a[i] = de_a[i] = pub[i] = 0;
	srand((unsigned)time(NULL));

// 平文とそのビット数を求める
	bit = input_a(a);

	if(bit == 0) return 0;

// key_gen で公開鍵と秘密鍵を作成
	key_gen(w,bit,&q,&r,pub);
	printf("INPUT = ");

	for (i = 0;i < bit;i ++)printf("%ld",a[i] );

	printf("\n");
// 秘密鍵の出力
	printf("----private_keys----\n");
	printf("W = {" );
	print_array(w,bit);
	printf("}\n" );

	for (i =0;i < bit;i ++) sum += w[i];

	printf("(Wsum = %ld)\n",sum );
	printf("q = %ld\n",q );
	printf("r = %ld\n",r );
	printf("\n");

// 公開鍵の出力
	printf("----public_keys----\n");
	printf("Public_key = {" );
	print_array(pub,bit);
// pub_key >> pub_out.txt
	fprint_array(pub,bit);
	printf("}\n");
	printf("\n");

// 暗号化->暗号文cの出力
	encryption(&c,a,pub,bit);
	printf("暗号文 c = %ld\n\n",c);
	FILE *fp;
	fp =fopen("out.txt","a");
	fprintf(fp,"%ld\n",c);
	fclose(fp);

// 復号化
// 逆元の計算、出力
	inv = inverse(r,q );
	printf("inverse %ld\n",inv);
	printf("c' = %ld\n",(c*inv)%q);
	decryption(c,r,q,w,de_a,bit,inv);

// 復号された結果の出力
	printf("\nRESULT = ");

	for (i = 0;i < bit;i ++) printf("%ld",de_a[i]);

	printf("\n");
	return 0;
}

void decryption(long int c,long int r,long int q,long int *w,long int *de,int n,long int inv) {
    long int r_sub = inv;
    long int s = (c*r_sub)%q;

    for (int i = n-1;i >= 0;i --) {
        if (s >= w[i]){
            s = s - w[i];
            de[i] = 1;
        } else {
            de[i] = 0;
        }
    }
}

long int inverse(long int r,long int q) {
	long int x = 1;
	long int r_sub = r;
	long int sub = 0;
	long int i = 0;

	while (1) {
		sub = r_sub - q;
        if (sub <= 0) {
			r_sub += r;
			x ++;
        } else if (sub > 1) {
			r_sub = sub + r;
			x ++;
        } else if (sub == 1) {
			break;
        }
	}
	return x;
}

void encryption(long int *c,long int *a,long int *pub,int n) {

	for (int i = 0;i < n;i ++) *c += a[i]*pub[i];

}

int input_a(long int *a) {
    int bit,i;
    char buf[N_MAX];
    printf("\n適当な2進数を入力してください。\n");

    if (!fgets(buf,N_MAX,stdin)) return 0;

	for (i = 0;i < N_MAX;i ++) {
		if (buf[i] != '1' && buf[i] != '0' && buf[i] != '\n') {
			printf("0or1を入力してください。\n");
			return input_a(a);
		}
        else if (buf[i] == '1') {
            a[i] = 1;
        } else if (buf[i] == '0'){
            a[i] = 0;
        } else if (buf[i] == '\n') {
            break;
        }
    }
    bit = i;
    printf("\n平文 a = %d bit \n",bit);
	return bit;
}

long int gcd(long int r,long int q) {
    long int c;
    c = q % r;
    while (c != 0) {
        q = r;
        r = c;
        c = q % r;
    }
    return r;
}

void key_gen (long int *w,int n,long int *q,long int *r,long int *pub) {
    long int i,j,sum,gcd_sub;
	i = j = sum = gcd_sub = 0;
	// w[i]の生成
	for (i = 0;i < n;i ++) {
        for (j = 0;j <= i;j ++) {
            sum += w[j] + rand()%5 + 1;
        }
        w[i] = sum;
        	sum = 0;
    }

    for (i =0;i < n;i ++) sum += w[i];

	// qの作成
    *q = sum + rand()%10 +1;

	// rの作成
    for (*r = *q/3;*r < *q;++ *r)  {
    	gcd_sub = gcd(*r,*q);

    	if (gcd_sub == 1) break;
	}

	//pub_key生成
    for (i = 0;i < n;i ++) ßpub[i] = ((*r) * w[i])%(*q);

}

//配列のファイル出力
void fprint_array(long int *array,int n) {
	FILE *fp1;
	fp1 = fopen("out.txt","a");

	for (int i = 0;i < n;i ++) fprintf(fp1,"%ld ",array[i] );

	fprintf(fp1,"\n");
	fclose (fp1);
}

//配列の中身を出力
void print_array(long int *array,int n) {
	for (int i = 0;i < n;i ++) {
        if (i == n -1) {
            printf("%ld ",array[i]);
        } else {
        	printf("%ld, ",array[i]);
        }
    }
}
