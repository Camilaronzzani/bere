#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_PRODUTOS 5
#define MAX_NOME 50
#define nome_arquivo "dados.txt"
#define tamanho_buffer 100

typedef struct
{
    int id;
    char nome[MAX_NOME];
    float preco;
    int quantidade;
} Produto;

Produto limpeza[MAX_PRODUTOS];
Produto alimentos[MAX_PRODUTOS];
Produto padaria[MAX_PRODUTOS];

int caixaAberto = 0, idGlobal = 1, contadorLimpeza = 0, contadorAlimentos = 0, contadorPadaria = 0;
float fundoDeCaixa = 0;
float totalVendas = 0;
FILE *arquivo;

// fazer funcao salvarproduto arquivo
// fazer funcao de editar estoque na saido do protudo(editar arquivo)-- mais complexo

void pega_hora_atual()
{
    time_t agora;
    struct tm *infoTempo;
    time(&agora);
    infoTempo = localtime(&agora);
    int hora = infoTempo->tm_hour;

    printf("\n===========================================\n");
    if (hora >= 5 && hora < 12)
        printf("Bom dia Dona Berê, Bem-vindo ao Mercadinho! ☀️\n");
    else if (hora >= 12 && hora < 18)
        printf("Boa tarde Dona Berê, Bem-vindo ao Mercadinho! 🌤️\n");
    else
        printf("Boa noite Dona Berê, Bem-vindo ao Mercadinho! 🌙\n");
}

void exibirMenu()
{
    printf("\n============================================\n");
    printf("           MENU MERCADINHO 🛍️              \n");
    printf("============================================\n");
    if (caixaAberto == 0)
    {
        printf("1. Abrir Caixa 💵\n");
        printf("2. Sair 🚪\n");
    }
    else
    {
        printf("1. Cadastrar Produto 📝\n");
        printf("2. Exibir Produtos 📦\n");
        printf("3. Realizar Compra 🛒\n");
        printf("4. Realizar Pagamento 💳\n");
        printf("5. Realizar Sangria 💸\n");
        printf("6. Fechar Caixa 🧾\n");
        printf("7. Sair 🚪\n");
    }

    printf("============================================\n");
    printf("Escolha uma opção: ");
}

void abrirCaixa()
{
    if (caixaAberto)
    {
        printf("\n");
    }
    else
    {
        do
        {
            system("clear");
            printf("\nDigite o valor inicial do caixa: R$");
            scanf("%f", &fundoDeCaixa);
            getchar();
            if (fundoDeCaixa <= 0)
            {
                printf("Valor inválido! Digite um valor positivo.\n");
            }
        } while (fundoDeCaixa <= 0);

        caixaAberto = 1;
        totalVendas = 0;
        printf("Caixa aberto com sucesso!\n");
    }
}

void cadastrarCategoria(int *contadorLimpeza, int *contadorAlimentos, int *contadorPadaria)
{
    int categoria, continuar;
    do
    {
        system("clear || cls");
        printf("\n===============================\n");
        printf("      Cadastro de Produto\n");
        printf("===============================");
        printf("\nEscolha a categoria do produto:\n");
        printf("1. Material de Limpeza\n");
        printf("2. Venda de Alimentos\n");
        printf("3. Padaria\n");
        printf("4. Voltar ao menu\n");
        printf("Categoria: ");
        if (scanf("%d", &categoria) != 1)
        {
            printf("Entrada inválida! Digite um número.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        switch (categoria)
        {
        case 1:
            cadastrarProduto(limpeza, contadorLimpeza);
            break;
        case 2:
            cadastrarProduto(alimentos, contadorAlimentos);
            break;
        case 3:
            cadastrarProduto(padaria, contadorPadaria);
            break;
        case 4:
            return;
        default:
            printf("Opção inválida!\n");
        }
    } while (categoria != 4);
}

void cadastrarProduto(Produto categoria[], int *contador)
{
    char escolha;
    int continuar;

    arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo para escrita");
        return;
    }

    printf("Programa para gravar dados em arquivo.\n");

    if (*contador >= MAX_PRODUTOS)
    {
        printf("Limite de produtos atingido!\n\n");
        fclose(arquivo);
        return;
    }

    printf("\nID do produto: %d\n", idGlobal);
    categoria[*contador].id = idGlobal;
    getchar();

    printf("Nome do produto: ");
    fgets(categoria[*contador].nome, MAX_NOME, stdin);
    categoria[*contador].nome[strcspn(categoria[*contador].nome, "\n")] = '\0';
    if (strlen(categoria[*contador].nome) <= 2)
    {
        printf("Vazio");
        getchar();
        return;
    }

    printf("Preço: R$");
    while (scanf("%f", &categoria[*contador].preco) != 1 || categoria[*contador].preco <= 0)
    {
        printf("Preço inválido! Digite um valor positivo: R$");
        while (getchar() != '\n')
            ;
    }
    getchar();

    printf("Quantidade: ");
    while (scanf("%d", &categoria[*contador].quantidade) != 1 || categoria[*contador].quantidade < 0)
    {
        printf("Quantidade inválida! Digite um número positivo: ");
        while (getchar() != '\n')
            ;
    }
    getchar();

    fprintf(arquivo, "ID: %d| Produto: %s| Preço: %.2f| Quantidade: %d\n",
            categoria[*contador].id,
            categoria[*contador].nome,
            categoria[*contador].preco,
            categoria[*contador].quantidade);

    fclose(arquivo);
    printf("Dados gravados com sucesso no arquivo '%s'.\n", nome_arquivo);

    (*contador)++;
    idGlobal++;

    printf("\nDeseja cadastrar outro produto? (S/N): ");
    scanf(" %c", &escolha);
    getchar();
    continuar = (escolha == 'S' || escolha == 's') ? 1 : 0;

    if (continuar == 1)
        cadastrarProduto(categoria, contador);

    system("clear");
    printf("\nProduto cadastrado com sucesso!");
}

//  melhorias que precisa: pegar diretamente do arquivo
void exibirProdutos(Produto categoria[], int contador, char *titulo)
{
    arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo para leitura");
        return 1;
    }

    printf("Programa para ler dados do arquivo '%s'.\n", nome_arquivo);
    printf("--- Conteúdo do arquivo ---\n");

    //falta pegar do arquivo
    int i;
    printf("\n===== %s =====\n", titulo);
    if (contador == 0)
    {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    for (i = 0; i < contador; i++)
    {
        char estoque_msg[20];
        if (categoria[i].quantidade == 0)
        {
            strcpy(estoque_msg, "(SEM ESTOQUE)");
        }
        else
        {
            sprintf(estoque_msg, "(%d unidades)", categoria[i].quantidade);
        }

        printf("ID: %d | %s - R$%.2f - %s\n",
               categoria[i].id, categoria[i].nome,
               categoria[i].preco, estoque_msg);
    }
}

// melhorias que precisa: exibir os produtos no estoque que pega diretamente do arquivo
void comprarProduto(Produto categoria[], int quantidadeProdutos, float *total)
{
    int idBuscado, quantidade;
    printf("\nDigite o ID do produto: ");
    scanf("%d", &idBuscado);

    for (int i = 0; i < quantidadeProdutos; i++)
    {
        if (categoria[i].id == idBuscado)
        {
            printf("Quantidade desejada: ");
            scanf("%d", &quantidade);

            if (!verificarEstoque(categoria[i], quantidade))
            {
                return;
            }

            categoria[i].quantidade -= quantidade;
            float valorCompra = quantidade * categoria[i].preco;
            *total += valorCompra;
            printf("Compra realizada! Total: R$%.2f\n", valorCompra);
            return;
        }
    }
    printf("Produto com ID %d não encontrado!\n", idBuscado);
}

// melhorias que precisa: vereficar
int verificarEstoque(Produto p, int quantidade)
{
    if (p.quantidade == 0)
    {
        printf("Produto sem estoque!\n");
        return 0;
    }
    if (p.quantidade < quantidade)
    {
        printf("Quantidade insuficiente em estoque!\n");
        return 0;
    }
    return 1;
}

// melhorias que seria legais: deixar fazer mais de uma forma de pagamento
void realizarPagamento(float totalLimpeza, float totalAlimentos, float totalPadaria)
{
    float totalGeral = totalLimpeza + totalAlimentos + totalPadaria;

    printf("\n===== Resumo da Compra =====\n");
    printf("Material de Limpeza: R$%.2f\n", totalLimpeza);
    printf("Alimentos: R$%.2f\n", totalAlimentos);
    printf("Padaria: R$%.2f\n", totalPadaria);
    printf("Total Geral: R$%.2f\n", totalGeral);

    int metodo;
    printf("\nEscolha a forma de pagamento:\n");
    printf("1 - Dinheiro\n");
    printf("2 - Cartão\n");
    printf("Método: ");
    scanf("%d", &metodo);

    if (metodo == 1)
    {
        float desconto = 0;
        if (totalGeral <= 50)
        {
            desconto = 0.05;
        }
        else if (totalGeral < 200)
        {
            desconto = 0.10;
        }
        else
        {
            printf("Informe o desconto (em %%): ");
            scanf("%f", &desconto);
            desconto /= 100;
        }

        totalGeral *= (1 - desconto);
        printf("Total com desconto: R$%.2f\n", totalGeral);

        float valorPago;
        printf("Valor recebido: R$");
        scanf("%f", &valorPago);

        if (valorPago < totalGeral)
        {
            printf("Valor insuficiente. Pagamento cancelado.\n");
            return;
        }

        float troco = valorPago - totalGeral;
        printf("Troco: R$%.2f\n", troco);
        totalVendas += totalGeral;
    }
    else if (metodo == 2)
    {
        int confirmacao;
        do
        {
            printf("Pagamento no cartão (1 - OK, 0 - Não OK): ");
            scanf("%d", &confirmacao);

            if (!confirmacao)
            {
                int trocarMetodo;
                printf("Pagamento não realizado.\n");
                printf("Trocar método? (1 - Sim / 0 - Tentar de novo): ");
                scanf("%d", &trocarMetodo);

                if (trocarMetodo == 1)
                {
                    realizarPagamento(totalLimpeza, totalAlimentos, totalPadaria);
                    return;
                }
            }
        } while (!confirmacao);

        totalVendas += totalGeral;
        printf("Pagamento no cartão confirmado.\n");
    }
    else
    {
        printf("Método inválido!\n");
    }
}

//
void realizarSangria()
{
    if (!caixaAberto)
    {
        printf("\nOperação não permitida: Caixa fechado!\n");
        printf("Por favor, abra o caixa primeiro.\n");
        return;
    }

    float valor;
    printf("\n========== SANGRIA DE CAIXA ==========\n");
    printf("Saldo total disponível: R$%.2f\n", fundoDeCaixa + totalVendas);

    //  valida o valor
    int tentativas = 0;
    while (tentativas < 3)
    {
        printf("\nDigite o valor a ser retirado: R$");

        if (scanf("%f", &valor) != 1)
        {
            printf("Valor inválido! Digite apenas números.\n");
            while (getchar() != '\n')
                ;
            tentativas++;
            continue;
        }
        getchar();

        if (valor <= 0)
        {
            printf("Valor deve ser positivo!\n");
            tentativas++;
            continue;
        }

        if (valor > (fundoDeCaixa + totalVendas))
        {
            printf("Saldo insuficiente! Saldo atual: R$%.2f\n", fundoDeCaixa + totalVendas);
            return;
        }
        break;
    }

    if (tentativas >= 3)
    {
        printf("Número máximo de tentativas excedido.\n");
        return;
    }

    if (valor <= fundoDeCaixa)
    {
        fundoDeCaixa -= valor;
    }
    else
    {
        float diferenca = valor - fundoDeCaixa;
        fundoDeCaixa = 0;
        totalVendas -= diferenca;
    }

    // confirma a operaçao
    printf("\nSangria realizada com sucesso!\n");
    printf("Valor retirado: R$%.2f\n", valor);
    printf("Novo saldo disponível: R$%.2f\n", fundoDeCaixa + totalVendas);
    printf("=================================\n");
}

// funcao suavee
void fecharCaixa(float totalLimpeza, float totalAlimentos, float totalPadaria)
{
    if (!caixaAberto)
    {
        system("clear");
        printf("\nO caixa já está fechado!\n");
    }
    else
    {
        caixaAberto = 0;

        printf("\n========== FECHAMENTO DE CAIXA ==========\n");
        printf("1. Valor de abertura: R$%.2f\n", fundoDeCaixa);
        printf("2. Faturamento total (apenas vendas): R$%.2f\n", totalVendas);
        printf("3. Faturamento por categoria:\n");
        printf("   a. Material de Limpeza: R$%.2f\n", totalLimpeza);
        printf("   b. Alimentos: R$%.2f\n", totalAlimentos);
        printf("   c. Padaria: R$%.2f\n", totalPadaria);
        printf("\nTotal em caixa (abertura + vendas): R$%.2f\n", fundoDeCaixa + totalVendas);
    }
}

// melhorias que foi feita: foi implementado o submenu e arrumado erro de aceitar letras e while melhorado
int main()
{
    int opcao, categoriaCompra, opcaoSub, sair;
    float totalLimpeza = 0, totalAlimentos = 0, totalPadaria = 0;
    setlocale(LC_ALL, "Portuguese_Brazil");

    pega_hora_atual();
    do
    {
        if (caixaAberto == 0)
        {
            exibirMenu();
            scanf("%d", &opcao);
            getchar();
        }
        if (opcao == 2)
        {
            if (caixaAberto)
            {
                printf("Você deve fechar o caixa antes de sair!\n");
                continue;
            }
            else
            {
                system("clear");
                sair = 1;
                printf("Saindo do sistema...\n Até Mais!\n");
                break;
            }
        }
        else if (opcao == 1)
        {
            abrirCaixa();

            exibirMenu();
            scanf("%d", &opcaoSub);
            getchar();
            switch (opcaoSub)
            {
            case 1:
                cadastrarCategoria(&contadorLimpeza, &contadorAlimentos, &contadorPadaria);
                break;
            case 2:
                exibirProdutos(limpeza, contadorLimpeza, "Material de Limpeza");
                exibirProdutos(alimentos, contadorAlimentos, "Venda de Alimentos");
                exibirProdutos(padaria, contadorPadaria, "Padaria");
                break;
            case 3:
                printf("\nEscolha a categoria:\n");
                printf("1. Material de Limpeza\n");
                printf("2. Venda de Alimentos\n");
                printf("3. Padaria\n");
                printf("Categoria: ");
                scanf("%d", &categoriaCompra);
                switch (categoriaCompra)
                {
                case 1:
                    comprarProduto(limpeza, contadorLimpeza, &totalLimpeza);
                    break;
                case 2:
                    comprarProduto(alimentos, contadorAlimentos, &totalAlimentos);
                    break;
                case 3:
                    comprarProduto(padaria, contadorPadaria, &totalPadaria);
                    break;
                default:
                    printf("Categoria inválida!\n");
                }
                break;
            case 4:
                realizarPagamento(totalLimpeza, totalAlimentos, totalPadaria);
                break;
            case 5:
                realizarSangria();
                break;
            case 6:
                fecharCaixa(totalLimpeza, totalAlimentos, totalPadaria);
                break;
            case 7:
                system("clear || cls");
                sair = 1;
                printf("Saindo do sistema...\n Até Mais!\n");
                break;
            default:
                system("clear || cls");
                printf("Opção inválida!\n");
                break;
            }
        }
        else if (opcao != 1)
        {
            printf("operacao invalida");
        }
    } while (sair != 1);

    return 0;
}
