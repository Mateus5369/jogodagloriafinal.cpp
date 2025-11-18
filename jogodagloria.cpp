#include <iostream>
#include <string> //permite usar .lenght()
#include <cstdlib>
#include <ctime>

using namespace std;

// Estrutura que representa um jogador
struct Jogador {
    string nome;                 // Nome do jogador
    int pos = 0;                 // Posição atual no tabuleiro
    bool perdeVez = false;       // Indica se perde a próxima vez
    bool esperaUltrapassagem = false; // Indica se está "preso" até ser ultrapassado
};

// Função que simula o lançamento de um dado entre 1 e 6
int dado() {
    return rand() % 6 + 1;
}

// Função que verifica se todos os outros jogadores já ultrapassaram o jogador j
bool todosUltrapassaram(const Jogador& j, Jogador jogadores[], int num) {
    for (int i = 0; i < num; i++) {
        // Ignora o próprio jogador
        if (jogadores[i].nome != j.nome) {
            if (jogadores[i].pos <= j.pos)
                return false; // Se alguém ainda não o ultrapassou, retorna falso
        }
    }
    return true; // Só chega aqui se TODOS estiverem à frente
}

// Função para limpar o ecrã (Windows ou Linux/macOS)
void limparEcra() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Função que mostra o tabuleiro e os jogadores
void mostrarTabuleiro(Jogador jogadores[], int num) {
    limparEcra();

    cout << "\n-------------------- TABULEIROZAO ---------------------\n";

    // Tabuleiro de 1 a 30
    for (int i = 1; i <= 30; i++) {
        bool temJogador = false;

        // Verifica se algum jogador está nesta casa
        for (int j = 0; j < num; j++) {
            if (jogadores[j].pos == i) {
                cout << "[" << jogadores[j].nome[0] << "]"; // Mostra inicial do jogador
                temJogador = true;
                break;
            }
        }

        // Se não tiver jogador, desenha o tipo da casa
        if (!temJogador) {
            if (i == 5) cout << "[⏩]";      // Avança 3
            else if (i == 10) cout << "[😔]"; // Perde vez
            else if (i == 15) cout << "[🔙]"; // Volta 3
            else if (i == 20) cout << "[🔄]"; // Joga outra vez
            else if (i == 25) cout << "[✋]"; // Espera ultrapassagem
            else cout << "[ ]";              // Casa normal
        }
    }

    // Legenda
    cout << "\nLegenda: [⏩]=Avança 3 | [😔]=Perde vez | [🔙]=Volta 3 | [🔄]=Joga outra | [✋]=Espera ultrapassagem\n";
}

int main() {
    srand(time(0)); // Inicializa o gerador aleatório

    int num;

    cout << "===== JOGO DA GLORIA =====\n";

    // Pergunta número de jogadores (1-5)
    do {
        cout << "Quantos jogadores? (1-5): ";
        cin >> num;
        cin.ignore(); // Limpa ENTER

        if (num < 1 || num > 5) {
            cout << "Número inválido! Escolhe entre 1 e 5.\n";
        }
    } while (num < 1 || num > 5);

    // Vetor de jogadores
    Jogador jogadores[5];

    // Ler nomes dos jogadores
    for (int i = 0; i < num; i++) {
        cout << "Nome do jogador " << i + 1 << ": ";
        getline(cin, jogadores[i].nome);
    }

    bool acabou = false; // Controla se o jogo já terminou
    int vez = 0;         // Índice do jogador da vez

    // Loop principal do jogo
    while (!acabou) {
        mostrarTabuleiro(jogadores, num);

        Jogador& atual = jogadores[vez]; // Jogador atual

        // Caso esteja preso à espera de ultrapassagem
        if (atual.esperaUltrapassagem && !todosUltrapassaram(atual, jogadores, num)) {
            cout << atual.nome << " ainda espera que todos o ultrapassem.\n";
        }
        // Caso tenha perdido a vez
        else if (atual.perdeVez) {
            cout << atual.nome << " perdeste a vez!\n";
            atual.perdeVez = false; // Limpa estado
        }
        else {
            atual.esperaUltrapassagem = false; // Já pode jogar
            cout << "\nÉ a vez de " << atual.nome << ". Carrega ENTER para lançar o dado.";
            cin.ignore();

            // Jogada
            int valor = dado();
            cout << atual.nome << " tirou " << valor << "!\n";
            cout << "Pressiona ENTER para continuar...";
            cin.ignore();

            // Atualiza posição
            atual.pos += valor;
            if (atual.pos > 30) atual.pos = 30;

            // Casas especiais
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

            // Se chegou ao fim
            if (atual.pos >= 30) {
                limparEcra();
                cout << "\n" << atual.nome << " chegou ao fim! 🏁 GG EZ!\n";
                acabou = true;
                break;
            }

            // Atualiza tabuleiro (menos quando é casa 20)
            if (atual.pos != 20)
                mostrarTabuleiro(jogadores, num);
        }

        // Passa a vez (exceto casa 20 que joga de novo)
        if (atual.pos != 20 || acabou) {
            vez = (vez + 1) % num;
        }
    }

    return 0;
}
