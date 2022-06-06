#include <stdio.h>
#include <math.h>
#include <string.h>


typedef double afunc(double);
int count_of_iters; // переменная, предназначенная для подсчета количества необходимых итераций для приближенного решение уравнений при поиске точек пересечения.

extern double f1(double);
extern double f2(double);
extern double f3(double);
extern double diff_f1(double);
extern double diff_f2(double);
extern double diff_f3(double);
extern double test_f1(double);
extern double test_f2(double);
extern double test_f3(double);
extern double test_diff_f1(double);
extern double test_diff_f2(double);
extern double test_diff_f3(double);

double root(afunc *f, afunc *g, afunc *diff_f, afunc *diff_g, double a, double b, double eps);
double integral (afunc *F, double start, double end, double eps);

double root(afunc *f, afunc *g, afunc *diff_f, afunc *diff_g, double a, double b, double eps) {
    double Fa = f(a) - g(a);
    double Fb = f(b) - g(b);
    double xres = 0;

    char sign = 0; // убывает
    double Fif1 = f((a + b) / 2) - g((a + b) / 2);
    double Fif2 = (Fa + Fb) / 2;
    if (((Fa < 0) && (Fif1 < Fif2)) || ((Fa > 0) && (Fif1 > Fif2))) {
        sign = 1; //возрастает
    }
    while (1) {
        count_of_iters++;
        Fa = f(a) - g(a);
        Fb = f(b) - g(b);

        if (sign == 1) {
            b = b - (Fb)/(diff_f(b) - diff_g(b));
            a = a - ((Fa)*(b-a)) / (Fb - Fa);
        } else {
            a = a - (Fa)/(diff_f(a) - diff_g(a));
            b = b - ((Fb)*(b-a)) / (Fb - Fa);
        }
        if (fabs(a - b) < 2*eps) {
            xres = (a+b) / 2;
            return xres;
        }
    }
}


double integral (afunc *F, double start, double end, double eps) {
    double diff = 1;
    double h = 0;
    double n = 2;
    h = (end - start) / n;
    double Fa0 = F(start);
    double Fa2n = F(end);
    double S_odd_prev = 0;
    double S_even_prev = 0;
    double S_odd_new = 0;
    double S_even_new = 0;
    double I_2n = 0;
    double I_n = 0;
    double Ifin = 0;
    S_even_prev = F(start + h);
    while (1) {
        n *= 2;
        h = h / 2;

        //calculating part of S_odd_new
        for (double i = start + h; i < end; i += 2*h) {
            double t = F(i);
            S_odd_new += t;
        }


        S_even_new = (S_even_prev + S_odd_prev);
        I_2n = (h*(Fa0 + Fa2n + 2 * S_even_new + 4 * S_odd_new))/3;
        diff = (fabs(I_2n - I_n)) / 15;
        if (diff < eps) {
            Ifin = I_2n;
            break;
        }
        I_n = I_2n;
        S_even_prev = S_even_new;
        S_odd_prev = S_odd_new;
        S_odd_new = 0;
        S_even_new = 0;
    }
    return Ifin;
}

int main (int argc, char *argv[]) {
    double point12;
    double point13;
    double point23;
    double area_f1;
    double area_f2;
    double area_f3;
    double final;
    int it_f1;
    int it_f2;
    int it_f3;
    point23 = root(f2, f3, diff_f2, diff_f3, -0.2, 0, 0.0001);
    it_f1 = count_of_iters;
    count_of_iters = 0;
    point12 = root(f1, f2, diff_f1, diff_f2, -1.9, -1.75, 0.0001);
    it_f2 = count_of_iters;
    count_of_iters = 0;
    point13 = root(f1, f3, diff_f1, diff_f3, 0.4, 0.5, 0.0001);
    it_f3 = count_of_iters;
    count_of_iters = 0;
    area_f1 = integral(f1, point13, point12, 0.0001);
    area_f2 = integral(f2, point23, point12, 0.0001);
    area_f3 = integral(f3, point13, point23, 0.0001);
    final = fabs(area_f1 - area_f2 - area_f3);
    for (int i = 0; i < argc; i++) {
        if (argc == 1) {
            printf("%lf\n", final);
        } else {
            double start, end, R;
            char symbol;
            double point;
            double abs;
            double rel;
            int num_of_func1, num_of_func2;
            double e1;
            int num_of_fun;
            double e2;
            if (!strcmp(argv[i], "--test-root") || !strcmp(argv[i], "-R")){
                sscanf(argv[i + 1], "%d%c%d%c%lf%c%lf%c%lf%c%lf", &num_of_func1, &symbol, &num_of_func2, &symbol, &start, &symbol, &end, &symbol, &e1, &symbol, &R);
                if ((num_of_func1 == 1 && num_of_func2 == 2) || (num_of_func1 == 2 && num_of_func2 == 1)){
                    point = root(test_f1, test_f2, test_diff_f1, test_diff_f2, start, end, e1);
                    abs = fabs(R - point);
                    rel = abs / R;
                    printf("%lf %lf %lf\n", point, abs, rel);
                }
                if ((num_of_func1 == 1 && num_of_func2 == 3) || (num_of_func1 == 3 && num_of_func2 == 1)){
                    point = root(test_f1, test_f3, test_diff_f1, test_diff_f3, start, end, e1);
                    abs = fabs(R - point);
                    rel = abs / R;
                    printf("%lf %lf %lf\n", point, abs, rel);
                }
                if ((num_of_func1 == 2 && num_of_func2 == 3) || (num_of_func1 == 3 && num_of_func2 == 2)){
                    point = root(test_f2, test_f3, test_diff_f2, test_diff_f3, start, end, e1);
                    abs = fabs(R - point);
                    rel = abs / R;
                    printf("%lf %lf %lf\n", point, abs, rel);
                }
            }

            if (!strcmp(argv[i], "--test-integral") || !strcmp(argv[i], "-I")){
                sscanf(argv[i + 1], "%d%c%lf%c%lf%c%lf%c%lf", &num_of_fun, &symbol, &start, &symbol, &end, &symbol, &e2, &symbol, &R);
                if (num_of_fun == 1){
                    point = integral (test_f1, start, end, e2);
                    abs = fabs(R - point);
                    rel = abs / R;
                    printf("%lf %lf %lf\n", point, abs, rel);
                }
                if (num_of_fun == 2){
                    point = integral (test_f2, start, end, e2);
                    abs = fabs(R - point);
                    rel = abs / R;
                    printf("%lf %lf %lf\n", point, abs, rel);
                }
                if (num_of_fun == 3){
                    point = integral (test_f3, start, end, e2);
                    abs = fabs(R - point);
                    rel = abs / R;
                    printf("%lf %lf %lf\n", point, abs, rel);
                }
            }
            if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h")) {
                printf("--help, -h\n--roots, -r\n--iterations, -i\n--test-root, -R\n--test-integral, -I\n");
            }

            if (!strcmp(argv[i], "--root") || !strcmp(argv[i], "-r")) {
                printf("roots: %lf %lf %lf\n", point12, point13, point23);
            }

            if (!strcmp(argv[i], "--iterations") || !strcmp(argv[i], "-i")) {
                printf("iterations: %d %d %d\n", it_f1, it_f2, it_f3);
            }
        }
    }
    return 0;
}
