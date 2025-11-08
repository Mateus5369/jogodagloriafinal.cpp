#include <iostream>
#include <vector> //Permite acesso aleatório rápido a qualquer elemento usando o operador subscrito [] ou iteradores
#include <string>
#include <cstdlib> //Gerir números, memória, aleatoriedade e controlo do programa.
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

bool todosUltrapassaram(const Jogador& j, const vector<Jogador>& jogadores) {
    for (const auto& outro : jogadores) {
        if (outro.nome != j.nome && outro.pos <= j.pos)
            return false;
    }
    return true;
}

void limparEcra() {
    // Sistema Windows
    #ifdef _WIN32
        system("cls");
    // Sistema Unix/Linux/MacOS
    #else
        system("clear");
    #endif
}

void mostrarTabuleiro(const vector<Jogador>& jogadores) {
    limparEcra(); // Limpa o ecrã antes de mostrar o tabuleiro atualizado
    
    cout << "\n-------------------- TABULEIROZAO ---------------------\n";
    for (int i = 1; i <= 30; i++) {
        bool temJogador = false;
        for (auto& j : jogadores) {
            if (j.pos == i) {
                cout << "[" << j.nome[0] << "]";
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
    cout << "\nLegenda: [⏩]=Avanca 3 | [😔]=Perde vez | [🔙]=Volta 3 | [🔄]=Joga outra | [✋]=Espera a ultrapassagem colega\n";
}

int main() {
    srand(time(0));

    int num;
    cout << "===== JOGO DA GLORIA =====\n";
    
    // Loop para garantir que o número de jogadores está entre 1 e 5
    do {
        cout << "Quantos jogadores? (1-5): ";
        cin >> num;
        cin.ignore();
        
        if (num < 1 || num > 5) {
            cout << "Numero invalido! Escolha entre 1 e 5 jogadores.\n";
        }
    } while (num < 1 || num > 5);

    vector<Jogador> jogadores(num);

    for (int i = 0; i < num; i++) {
        cout << "Nome do jogador " << i + 1 << ": ";
        getline(cin, jogadores[i].nome);
    }

    bool acabou = false;
    int vez = 0;

    while (!acabou) {
        mostrarTabuleiro(jogadores);

        Jogador& atual = jogadores[vez];

        // se estiver à espera dos outros
        if (atual.esperaUltrapassagem && !todosUltrapassaram(atual, jogadores)) {
            cout << atual.nome << " ainda espera que todos o ultrapassem.\n";
        }
        else if (atual.perdeVez) {
            cout << atual.nome << " perdeste a vez!\n";
            atual.perdeVez = false;
        }
        else {
            atual.esperaUltrapassagem = false;
            cout << "\nE a vez de " << atual.nome << ". Clica ENTER para lancar o dado.";
            cin.ignore();

            int valor = dado();
            cout << atual.nome << " tirou " << valor << "!\n";
            cout << "Pressiona ENTER para continuar...";
            cin.ignore();

            atual.pos += valor;
            if (atual.pos > 30) atual.pos = 30;

            // casas especiais
            switch (atual.pos) {
                case 5:
                    cout << "Casa especial (+3): avanca 3 casas!\n";
                    atual.pos += 3;
                    if (atual.pos > 30) atual.pos = 30;
                    break;
                case 10:
                    cout << "Casa especial (P): perde a proxima jogada!\n";
                    atual.perdeVez = true;
                    break;
                case 15:
                    cout << "Casa especial (-3): volta 3 casas!\n";
                    atual.pos -= 3;
                    if (atual.pos < 1) atual.pos = 1;
                    break;
                case 20:
                    cout << "Casa especial (J): joga novamente!\n";
                    break;
                case 25:
                    cout << "Casa especial (E): espera ate todos o ultrapassarem!\n";
                    atual.esperaUltrapassagem = true;
                    break;
            }

            if (atual.pos >= 30) {
                limparEcra();
                cout << "\n " << atual.nome << " 🏁 GG ez\n";
                acabou = true;
                break;
            }
            
            // Se não ganhou, mostra o tabuleiro atualizado
            if (atual.pos != 20) { // Se não for a casa "joga novamente"
                mostrarTabuleiro(jogadores);
            }
        }

        // Avança para o próximo jogador, exceto se for casa "joga novamente"
        if (atual.pos != 20 || acabou) {
            vez = (vez + 1) % num;
        }
    }

    return 0;
}