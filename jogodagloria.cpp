#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// Estrutura que representa um jogador
struct Jogador {
    string nome;               // Nome do jogador
    int pos = 0;               // Posição atual no tabuleiro
    bool perdeVez = false;     // Indica se o jogador perde a próxima jogada
    bool esperaUltrapassagem = false; // Indica se o jogador deve esperar até ser ultrapassado
};

// Função que simula o lançamento de um dado (1 a 6)
int dado() {
    return rand() % 6 + 1;
}

// Função que verifica se todos os outros jogadores já ultrapassaram o jogador j
bool todosUltrapassaram(const Jogador& j, Jogador jogadores[], int num) {
    for (int i = 0; i < num; i++) {
        if (jogadores[i].nome != j.nome && jogadores[i].pos <= j.pos)
            return false; // Se algum jogador não o ultrapassou, retorna falso
    }
    return true;
}

// Função que limpa o ecrã (diferente comando para Windows e Linux/macOS)
void limparEcra() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Função que mostra o estado atual do tabuleiro e dos jogadores
void mostrarTabuleiro(Jogador jogadores[], int num) {
    limparEcra(); // Limpa o ecrã a cada atualização

    cout << "\n-------------------- TABULEIROZAO ---------------------\n";
    for (int i = 1; i <= 30; i++) { // O tabuleiro tem 30 casas
        bool temJogador = false;

        // Verifica se há algum jogador nesta casa
        for (int j = 0; j < num; j++) {
            if (jogadores[j].pos == i) {
                cout << "[" << jogadores[j].nome[0] << "]"; // Mostra a inicial do jogador
                temJogador = true;
                break;
            }
        }

        // Se não houver jogador na casa, mostra o tipo de casa especial
        if (!temJogador) {
            if (i == 5) cout << "[⏩]";      // Avança 3 casas
            else if (i == 10) cout << "[😔]"; // Perde a próxima vez
            else if (i == 15) cout << "[🔙]"; // Volta 3 casas
            else if (i == 20) cout << "[🔄]"; // Joga novamente
            else if (i == 25) cout << "[✋]"; // Espera até ser ultrapassado
            else cout << "[ ]";              // Casa normal
        }
    }

    // Legenda para o jogador entender os símbolos
    cout << "\nLegenda: [⏩]=Avanca 3 | [😔]=Perde vez | [🔙]=Volta 3 | [🔄]=Joga outra | [✋]=Espera ultrapassagem\n";
}

int main() {
    srand(time(0)); // Inicializa o gerador de números aleatórios

    int num;
    cout << "===== JOGO DA GLORIA =====\n";

    // Pergunta quantos jogadores vão jogar (entre 1 e 5)
    do {
        cout << "Quantos jogadores? (1-5): ";
        cin >> num;
        cin.ignore(); // Limpa o buffer do ENTER

        if (num < 1 || num > 5) {
            cout << "Numero invalido! Escolhe entre 1 e 5 jogadores.\n";
        }
    } while (num < 1 || num > 5);

    Jogador jogadores[5]; // Máximo de 5 jogadores

    // Lê o nome de cada jogador
    for (int i = 0; i < num; i++) {
        cout << "Nome do jogador " << i + 1 << ": ";
        getline(cin, jogadores[i].nome);
    }

    bool acabou = false; // Indica se o jogo terminou
    int vez = 0;         // Indica de quem é a vez de jogar

    // Loop principal do jogo
    while (!acabou) {
        mostrarTabuleiro(jogadores, num);

        Jogador& atual = jogadores[vez]; // Jogador atual da jogada

        // Caso o jogador esteja à espera de ser ultrapassado
        if (atual.esperaUltrapassagem && !todosUltrapassaram(atual, jogadores, num)) {
            cout << atual.nome << " ainda espera que todos o ultrapassem.\n";
        }
        // Caso o jogador tenha perdido a vez
        else if (atual.perdeVez) {
            cout << atual.nome << " perdeste a vez!\n";
            atual.perdeVez = false; // Reseta o estado para próxima rodada
        }
        else {
            atual.esperaUltrapassagem = false; // Já pode jogar novamente
            cout << "\nÉ a vez de " << atual.nome << ". Carrega ENTER para lançar o dado.";
            cin.ignore();

            // Lança o dado
            int valor = dado();
            cout << atual.nome << " tirou " << valor << "!\n";
            cout << "Pressiona ENTER para continuar...";
            cin.ignore();

            // Atualiza posição do jogador
            atual.pos += valor;
            if (atual.pos > 30) atual.pos = 30;

            // Casas especiais e seus efeitos
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

            // Verifica se o jogador chegou ao fim do tabuleiro
            if (atual.pos >= 30) {
                limparEcra();
                cout << "\n" << atual.nome << " chegou ao fim! 🏁 GG EZ!\n";
                acabou = true; // Termina o jogo
                break;
            }

            // Atualiza o tabuleiro se não for casa de "joga novamente"
            if (atual.pos != 20) {
                mostrarTabuleiro(jogadores, num);
            }
        }

        // Passa a vez para o próximo jogador (se não for casa 20)
        if (atual.pos != 20 || acabou) {
            vez = (vez + 1) % num;
        }
    }

    return 0;
}
