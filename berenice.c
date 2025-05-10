#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <stdlib.h>

#define MAX_PRODUTOS 5
#define MAX_NOME 50
#define nome_arquivo "dados.txt"
#define tamanho_buffer 100

// melhorias em geral fazer sangraria

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

// melhorias que precisa:
// fazer funcao salvarproduto arquivo
// fazer funcao de editar estoque na saido do protudo(editar arquivo)-- mais complexo

// funcao suave
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

// funcao suave
//  melhorias feitas: adicionado if de verificao de caixa aberto para o submenu
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
        printf("5. Fechar Caixa 🧾\n");
        printf("6. Sair 🚪\n");
    }

    printf("============================================\n");
    printf("Escolha uma opção: ");
}

// funcao suave
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

// exeluido funcao de deseija continuar e colocado dentro da funcao cadastrarProduto, pois nao é usado mais nem umlugar

// melhorias que precisa nao aceitar letras
// arrumado maizomenos ele nao aceita porem sai tudo
void cadastrarCategoria(int *contadorLimpeza, int *contadorAlimentos, int *contadorPadaria)
{
    int categoria, continuar;
    do
    {
        system("clear");
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
                ;   // limpa buffer
            return; // volta ao início do loop
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
        }

    } while (continuar || categoria == 1 && categoria == 2 && categoria == 3);
}

// funcao suave amemmmmmmmmmmmm
//  melhorias que precisa: salvar no arquivo
void cadastrarProduto(Produto categoria[], int *contador)
{
    char escolha;
    int continuar;

    FILE *arquivo;
    arquivo = fopen(nome_arquivo, "a");
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

    printf("Preço: R$");
    scanf("%f", &categoria[*contador].preco);
    getchar();

    printf("Quantidade: ");
    scanf("%d", &categoria[*contador].quantidade);
    getchar();

    fprintf(arquivo, "ID: %d; Produto: %s; Preço: %.2f; Quantidade: %d\n",
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
    printf("\nProduto cadastrado com sucesso!\n");
}

//----------- NAO AJUSTADO -------------------

//  melhorias que precisa: pegar diretamente do arquivo
void exibirProdutos(Produto categoria[], int contador, char *titulo)
{
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

// melhorias q precisa:
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

// melhorias que precisa: pegar do arquivo e exibir os produtos no estoque
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

// melhorias que precisa:
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

// funcao suave
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
                fecharCaixa(totalLimpeza, totalAlimentos, totalPadaria);
                break;
            case 6:
                system("clear");
                sair = 1;
                printf("Saindo do sistema...\n Até Mais!\n");
                break;
            default:
                system("clear");
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
