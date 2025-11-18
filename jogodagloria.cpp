#include <iostream>
#include <string> //permite usar funções do tipo .length()
#include <cstdlib>
#include <ctime>

using namespace std;

struct Jogador {
    string nome;
    int pos = 0;
    bool perdeVez = false;
    bool esperaUltrapassagem = false;
};

int dado() {
    return rand() % 6 + 1;
}

bool todosUltrapassaram(const Jogador& j, Jogador jogadores[], int num) {
    for (int i = 0; i < num; i++) {
        if (jogadores[i].nome != j.nome && jogadores[i].pos <= j.pos)
            return false;
    }
    return true;
}

void limparEcra() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void mostrarTabuleiro(Jogador jogadores[], int num) {
    limparEcra();

    cout << "\n-------------------- TABULEIROZAO ---------------------\n";
    for (int i = 1; i <= 30; i++) {
        bool temJogador = false;

        for (int j = 0; j < num; j++) {
            if (jogadores[j].pos == i) {
                cout << "[" << jogadores[j].nome[0] << "]";
                temJogador = true;
                break;
            }
        }

        if (!temJogador) {
            if (i == 5) cout << "[⏩]";
            else if (i == 10) cout << "[😔]";
            else if (i == 15) cout << "[🔙]";
            else if (i == 20) cout << "[🔄]";
            else if (i == 25) cout << "[✋]";
            else cout << "[ ]";
        }
    }
    cout << "\nLegenda: [⏩]=Avanca 3 | [😔]=Perde vez | [🔙]=Volta 3 | [🔄]=Joga outra | [✋]=Espera ultrapassagem\n";
}

int main() {
    srand(time(0));

    int num;
    cout << "===== JOGO DA GLORIA =====\n";

    do {
        cout << "Quantos jogadores? (1-5): ";
        cin >> num;
        cin.ignore();

        if (num < 1 || num > 5) {
            cout << "Numero invalido! Escolhe entre 1 e 5 jogadores.\n";
        }
    } while (num < 1 || num > 5);

    Jogador jogadores[5];

    for (int i = 0; i < num; i++) {
        cout << "Nome do jogador " << i + 1 << ": ";
        getline(cin, jogadores[i].nome);
    }

    bool acabou = false;
    int vez = 0;

    while (!acabou) {
        mostrarTabuleiro(jogadores, num);

        Jogador& atual = jogadores[vez];

        if (atual.esperaUltrapassagem && !todosUltrapassaram(atual, jogadores, num)) {
            cout << atual.nome << " ainda espera que todos o ultrapassem.\n";
        }
        else if (atual.perdeVez) {
            cout << atual.nome << " perdeste a vez!\n";
            atual.perdeVez = false;
        }
        else {
            atual.esperaUltrapassagem = false;

            cout << "\nÉ a vez de " << atual.nome << ". Carrega ENTER para lançar o dado.";
            cin.ignore();

            int valor = dado();
            cout << atual.nome << " tirou " << valor << "!\n";
            cout << "Pressiona ENTER para continuar...";
            cin.ignore();

            atual.pos += valor;
            if (atual.pos > 30) atual.pos = 30;

            switch (atual.pos) {
                case 5:
                    cout << "Casa especial (+3): avança 3 casas!\n";
                    atual.pos += 3;
                    if (atual.pos > 30) atual.pos = 30;
                    break;
                case 10:
                    cout << "Casa especial (😔): perde a próxima jogada!\n";
                    atual.perdeVez = true;
                    break;
                case 15:
                    cout << "Casa especial (-3): volta 3 casas!\n";
                    atual.pos -= 3;
                    if (atual.pos < 1) atual.pos = 1;
                    break;
                case 20:
                    cout << "Casa especial (🔄): joga novamente!\n";
                    break;
                case 25:
                    cout << "Casa especial (✋): espera até todos te ultrapassarem!\n";
                    atual.esperaUltrapassagem = true;
                    break;
            }

            if (atual.pos >= 30) {
                limparEcra();
                cout << "\n" << atual.nome << " chegou ao fim! 🏁 GG EZ!\n";
                acabou = true;
                break;
            }

            if (atual.pos != 20) {
                mostrarTabuleiro(jogadores, num);
            }
        }

        if (atual.pos != 20 || acabou) {
            vez = (vez + 1) % num;
        }
    }

    return 0;
}
