#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char estado[64];
    char codigo[32];
    char nome[128];
    long long populacao;
    double area;
    double pib;
    int pontosTuristicos;
} Carta;

typedef enum {
    POPULACAO = 0,
    AREA = 1,
    PIB = 2,
    PONTOS = 3,
    DENSIDADE = 4
} Atributo;

void limparEntrada() {
    int c; while ((c = getchar()) != '\n' && c != EOF) {}
}

void lerString(const char* rotulo, char* dest, size_t tam) {
    printf("%s", rotulo);
    if (fgets(dest, (int)tam, stdin) == NULL) {
        dest[0] = '\0';
        return;
    }
    size_t n = strlen(dest);
    if (n > 0 && dest[n - 1] == '\n') dest[n - 1] = '\0';
}

long long lerLongLong(const char *rotulo) {
    long long v; int ok = 0;
    do {
        printf("%s", rotulo);
        if (scanf("%lld", &v) == 1) ok = 1; else { printf("Entrada inválida. Tente novamente.\n"); }
        limparEntrada();
    } while (!ok);
    return v;
}

double lerDouble(const char *rotulo) {
    double v; int ok = 0;
    do {
        printf("%s", rotulo);
        if (scanf("%lf", &v) == 1) ok = 1; else { printf("Entrada inválida. Tente novamente.\n"); }
        limparEntrada();
    } while (!ok);
    return v;
}

int lerIntIntervalo(const char* rotulo, int min, int max) {
    int v; int ok = 0;
    do {
        printf("%s", rotulo);
        if (scanf("%d", &v) == 1 && v >= min && v <= max) ok = 1; else {
            printf("Opção inválida. Digite um valor entre %d e %d.\n", min, max);
        }
        limparEntrada();
    } while (!ok);
    return v;
}

double densidade(const Carta* c) {
    if (c->area <= 0.0) return 0.0;
    return (double)c->populacao / c->area;
}

double valorAtributo(const Carta* c, Atributo a) {
    switch (a) {
    case POPULACAO: return (double)c->populacao;
    case AREA: return c->area;
    case PIB: return c->pib;
    case PONTOS: return (double)c->pontosTuristicos;
    case DENSIDADE: return densidade(c);
    default: return 0.0;
    }
}

int compararCartas(const Carta *carta1, const Carta *carta2, Atributo atributo) {
    double v1 = valorAtributo(carta1, atributo);
    double v2 = valorAtributo(carta2, atributo);

    if (atributo == DENSIDADE) {
        if (v1 < v2) return 1;
        if (v1 > v2) return -1;
        return 0;
    }
    else {
        if (v1 > v2) return 1;
        if (v1 < v2) return -1;
        return 0;
    }
}

const char* nomeAtributo(Atributo a) {
    switch (a) {
    case POPULACAO: return "Populacao";
    case AREA: return "Area";
    case PIB: return "PIB";
    case PONTOS: return "Pontos Turisticos";
    case DENSIDADE: return "Densidade Populacional";
    default: return "";
    }
}

void cadastrarCarta(Carta* carta, const char* titulo) {
    printf("Cadastro da %s \n", titulo);
    lerString("Estado/Pais: ", carta->estado, sizeof(carta->estado));
    lerString("Codigo da carta: ", carta->codigo, sizeof(carta->codigo));
    lerString("Nome da cidade: ", carta->nome, sizeof(carta->nome));
    carta->populacao = lerLongLong("Populacao: ");
    carta->area = lerDouble("Area (km2): ");
    carta->pib = lerDouble("PIB: ");
    carta->pontosTuristicos = (int)lerLongLong("Pontos Turisticos: ");
}

void exibirCarta(const Carta* c) {
    printf("\n============= CARTA =============\n");
    printf("Estado/Pais : %s\n", c->estado);
    printf("Codigo : %s\n", c->codigo);
    printf("Cidade : %s\n", c->nome);
    printf("Populacao : %lld hab\n", c->populacao);
    printf("Area : %.2f km2\n", c->area);
    printf("PIB : %.2f\n", c->pib);
    printf("Pontos Tur. : %d\n", c->pontosTuristicos);
    printf("Densidade : %.4f hab/km2\n", densidade(c));
    printf("\n");
}

Atributo menuAtributo() {
    printf("\nEscolha o atributo para comparar:\n");
    printf("[0] Populacao\n");
    printf("[1] Area\n");
    printf("[2] PIB\n");
    printf("[3] Pontos Turisticos\n");
    printf("[4] Densidade Populacional\n");
    int op = lerIntIntervalo("Opcao: ", 0, 4);
    return (Atributo)op;
}

void mostrarResultado(const Carta* c1, const Carta* c2, int res, Atributo a) {
    printf("\n===== Resultado (%s) =====\n");
    printf("Atributo: %s\n", nomeAtributo(a));
    if (res > 0) {
        printf("Vencedora: %s (%s)\n", c1->nome, c1->codigo);
    }
    else if (res < 0) {
        printf("Vencedora: %s (%s)\n", c2->nome, c2->codigo);
    }
    else {
        printf("Empate! Ambas as cartas têm o mesmo desempenho no atributo.\n");
    }
}

int main() {

    Carta carta1, carta2;
    cadastrarCarta(&carta1, "Carta 1");
    cadastrarCarta(&carta2, "Carta 2");

    exibirCarta(&carta1);
    exibirCarta(&carta2);

    printf("\n========\n");
    Atributo primeiro = menuAtributo();
    Atributo segundo = menuAtributo();

    double v1p = valorAtributo(&carta1, primeiro);
    double v2p = valorAtributo(&carta2, primeiro);

    int resPrim = (primeiro == DENSIDADE) 
        ? (v1p < v2p) ? 1 : (v1p > v2p) ? -1 : 0 
            : (v1p > v2p) ? 1 : (v1p < v2p) ? -1 : 0;

    int resFinal = (resPrim != 0) 
        ? resPrim : 
        compararCartas(&carta1, &carta2, segundo);

    if (resPrim == 0) {
        printf("Empate no atributo primario (%s). Desempatado por %s \n", nomeAtributo(primeiro), nomeAtributo(segundo));
    }

    if (resFinal == 0) {
        printf("\n==== Resultado ====\n");
        printf("Empate! Mesmo desempenho em %s e %s. \n", nomeAtributo(primeiro), nomeAtributo(segundo));
    }
    else {
        mostrarResultado(&carta1, &carta2, resFinal, (resPrim != 0 ? primeiro : segundo));
    }

    system("pause");
    return 0;
}
