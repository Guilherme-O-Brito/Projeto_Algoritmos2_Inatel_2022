#include <iostream>
#include <fstream>
#include <cstring>
#include <list>
#include <windows.h>
#include <conio.h>

using namespace std;

struct words{

    char ING[50];
    char PT_BR[50];
    char vocab[50];

};

struct treenode{

    words *info; // raiz
    treenode *esq; // sub-arvore esquerda
    treenode *dir; //sub - arvore direita
    
};

typedef treenode *treenodeptr; // definindo uma variavel tipo ponteiro de treenode 

void tInsere(treenodeptr &p, char vocab[50], char PT_BR[50], char ING[50]){
	
    if (p == NULL){ // insere na raiz

		p = new treenode; //cria novo no na arvore
		p->info = new words; //cria novo objeto para as traducoes
        //copia as strings lidas para o no novo
        strcpy(p->info->vocab,vocab);
        strcpy(p->info->PT_BR,PT_BR);
        strcpy(p->info->ING,ING);
        //inicia os nos direito e esquerdo como NULL
		p->esq = NULL;
		p->dir = NULL;

	}else if(strcmp(p->info->vocab ,vocab) > 0 ) // insere na subarvore esquerda
		tInsere(p->esq, vocab, PT_BR, ING);
	else // insere na subarvore direita
		tInsere(p->dir, vocab, PT_BR, ING);
}

treenodeptr tPesq(treenodeptr p, char palavra[50]){

    if(p == NULL)
        return NULL; //retorna null caso o no nao exista

    if(strcmp(palavra,p->info->vocab) == 0)
        return p; //retorna ponteiro para o no referente a palavra

    //procura nos proximos nos usando recursividade
    if(strcmp(p->info->vocab ,palavra) > 0 )
        return tPesq(p->esq,palavra); 
    else
        return tPesq(p->dir,palavra);

}

void emOrdem(treenodeptr arvore){
    //mostra toda o conteudo da arvore pelo percurso em ordem
    if (arvore != NULL){

        emOrdem(arvore->esq);

        cout << "vocabulario: " << arvore->info->vocab << endl;
        cout << "Portugues: " << arvore->info->PT_BR << endl;
        cout << "Ingles: " << arvore->info->ING << endl;

        emOrdem(arvore->dir);

    }

}

void tRemove(treenodeptr &p, const char* vocab){
    //remove algum no da arvore
    if (p == NULL){
        return;
    }
    if (strcmp(p->info->vocab, vocab) > 0){
        tRemove(p->esq, vocab);
    } else if (strcmp(p->info->vocab, vocab) < 0){
        tRemove(p->dir, vocab);
    } else {
        if (p->esq == NULL && p->dir == NULL){
            delete p->info;
            delete p;
            p = NULL;
        } else if (p->esq == NULL){
            treenodeptr temp = p;
            p = p->dir;
            delete temp->info;
            delete temp;
        } else if (p->dir == NULL){
            treenodeptr temp = p;
            p = p->esq;
            delete temp->info;
            delete temp;
        } else {
            treenodeptr temp = p->dir;
            while (temp->esq != NULL){
                temp = temp->esq;
            }
            strcpy(p->info->vocab, temp->info->vocab);
            tRemove(p->dir, temp->info->vocab);
        }
    }

}

void treeList(list<string> &q, treenodeptr arvore){
    //transcreve a arvore numa lista usando o percurso em ordem
    if(arvore != NULL){

        treeList(q,arvore->esq);

        q.push_back(arvore->info->vocab);
        q.push_back(arvore->info->PT_BR);
        q.push_back(arvore->info->ING);

        treeList(q,arvore->dir);

    }

}

void tDestruir (treenodeptr &arvore){
    //limpa memoria usada na arvore
	if (arvore != NULL){

		tDestruir(arvore->esq);
		tDestruir(arvore->dir);
        delete arvore->info;
		delete arvore;

	}

	arvore = NULL;
}

void read(int &cont, treenodeptr &arvore){
    //le o arquivo de texto e insere o conteudo na arvore
    ifstream arqin; //handle para leitura de arquivo
    char ING[50], PT_BR[50], vocab[50];
    
    //abrindo o arquivo para leitura
	arqin.open("palavras.txt",ifstream::in);

    while(!arqin.eof()){

        arqin.getline(vocab,50);
        arqin.getline(PT_BR,50);
        arqin.getline(ING,50);
        tInsere(arvore,vocab,PT_BR,ING);
        cont++;

    }

    arqin.close(); //fecha o arquivo de leitura

}

void write(treenodeptr arvore){
    //escreve o conteudo da arvore no arquivo de texto
    list<string> q;
    list<string>::iterator p; //declarando ponteiro p para interagir com as listas

    treeList(q,arvore);

    ofstream arqout; //handle para escrever no arquivo

	//abrindo arquivo para escrita
	arqout.open("palavras.txt", ofstream::out);

    p = q.begin();

    while(p != q.end()){

        arqout <<  *p;

        p++;

        if(p != q.end())
            arqout << endl;

    }

    arqout.close();

    //limpa memoria usada pela lista
	while(!q.empty())
		q.pop_front();

}

void insere(treenodeptr &p){
    //solicida dados para criacao de um novo no na arvore
    char vocab[50], ING[50], PT_BR[50];

    cout<<"Insira o nome da palavra nome vocabulario novo: "<<endl;
    cin.getline(vocab,50);

    cout<<"Insira o nome da palavra em ingles: "<<endl;
    cin.getline(ING,50);

    cout<<"Insira o nome da palavra em portugues: "<<endl;
    cin.getline(PT_BR,50);

    tInsere(p, vocab, PT_BR, ING);

}

void tOrdenaPorTamanho(treenodeptr arvore, list<string> &lista){
    //ordena as palavras por tamanho em uma lista
    if (arvore != NULL) {
        tOrdenaPorTamanho(arvore->esq, lista);
        lista.push_back(arvore->info->vocab);
        tOrdenaPorTamanho(arvore->dir, lista);
    }

}

bool compararTamanhoString(const std::string& a, const std::string& b) {
    return a.size() < b.size();
}

void ordenarPorTamanho(treenodeptr arvore){

    list<string> listaPalavras;
    list<string>::iterator p;

    tOrdenaPorTamanho(arvore, listaPalavras);
    listaPalavras.sort(compararTamanhoString);
    
    cout << "Palavras ordenadas por tamanho:" << endl;
    for (p = listaPalavras.begin();p != listaPalavras.end();p++) {
        cout << *p << endl;
    }

    //limpa memoria usada pela lista
	while(!listaPalavras.empty())
		listaPalavras.pop_front();

}

int contarPalavrasMesmoTamanho(treenodeptr arvore) {
    //retorna quantidade de palavras com mesmo tamanho de sua traducao dentro da arvore
    if (arvore == NULL) {
        return 0;
    }

    int contador = 0;
    if (strlen(arvore->info->vocab) == strlen(arvore->info->PT_BR)) {
        contador = 1;
    }

    contador += contarPalavrasMesmoTamanho(arvore->esq);
    contador += contarPalavrasMesmoTamanho(arvore->dir);

    return contador;
}

void perc1(char palavra[50], treenodeptr arvore, int &em, int &em2, bool &find){
   //contabiliza passos ate chegar na palavra desejada pelo percurso em ordem
	if (arvore != NULL){

		perc1(palavra, arvore->esq,em, em2,find);
		em++;
		if(strcmp(arvore->info->ING,palavra) == 0){
		 
            em2 = em;
            find = true;
        
        }    
		perc1(palavra, arvore->dir, em,em2,find);
	}

}

void perc2(char palavra[50], treenodeptr arvore,int &pos, int &pos2){
    //contabiliza passos ate chegar na palavra desejada pelo percurso pos ordem
	if (arvore != NULL){
		
		perc2(palavra, arvore->esq,pos, pos2);
		perc2(palavra, arvore->dir,pos, pos2);
		pos++;
		if(strcmp(arvore->info->ING,palavra) == 0)
			pos2 = pos;
	
	}

}

void perc3(char palavra[50], treenodeptr arvore, int &pre, int &pre2){
    //contabiliza passos ate chegar na palavra desejada pelo percurso pre ordem
	if (arvore != NULL){

		pre++;
		if(strcmp(arvore->info->ING,palavra) == 0)
			pre2 = pre;
	
		perc3(palavra, arvore->esq,pre, pre2);
		perc3(palavra, arvore->dir,pre, pre2);

	}

}

void percursoRapido(treenodeptr arvore, char palavra[50]){
    //calcula e mostra na tela qual percurso mais rapido nessa arvore ate a palavra buscada
    int em2 = 0; 
	int pos2 = 0;
	int pre2 = 0;
	int em = 0; 
	int pos = 0; 
	int pre = 0;
    bool find = false; //sinaliza que a palavra foi ou nao encontrada na arvore
	perc1(palavra,arvore,em,em2,find);
	perc2(palavra, arvore,pos,pos2);
	perc3(palavra, arvore,pre,pre2);
	
    if(find)
        if(pre2 <= pos2 && pre2 <= em2)
            cout << "Pre-ordem" << endl;

        else if(pos2 <= pre2 && pos2 <= em2)
            cout << "Pos-ordem" << endl;
            
        else
            cout << "Em-ordem" << endl;
    else 
        cout << "Palavra nao encontrada no vocabulario" << endl;        

}

void menu(){
    //escreve na tela o menu de opcoes
    cout << "           Tradutor de Linguagens            " << endl; 
    cout << "pressione o numero correspondente a funcao que deseja utilizar" << endl << endl;
    cout << "1 - Inserir nova palavra no tradutor" << endl;
    cout << "2 - Buscar por uma palavra especifica" << endl;
    cout << "3 - Remover alguma palavra do vocabulario" << endl;
    cout << "4 - Ordenar por tamanho das palavras" << endl;
    cout << "5 - Verificar quantas palavras no vocab tem mesmo tamanho que sua traducao em portugues" << endl;
    cout << "6 - procurar uma palavra e descobrir qual percurso mais rapido ate ela" << endl;
    cout << "7 - Listar todas as palavras no vocabulario" << endl;
    cout << "8 - Finalizar" << endl;

}

int main(){

    treenodeptr vocab = NULL; //declarando a arvore vocab
    treenodeptr p = NULL; //ponteiro para acesso na arvore
    int cont = 0; //variavel para determinar quantas palavras 
    int contador; //variavel auxiliar
    int option; //opcao escolhida pelo usuario
    char palavra[50]; //vetor de char para auxiliar nas funcoes

    read(cont, vocab); //le o arquivo com as traducoes e cria a arvore  

    menu(); //inicia o menu do app

    option = getch(); //identifica opcao escolhida

    while(option != '8'){

        switch(option){

            case '1':
                system("cls"); //limpa tela
                insere(vocab); //insere palavra no vocabulario
                write(vocab);
                system("pause"); //pausa pra leitura
                system("cls"); //limpa tela
                menu(); //gera menu
                break;

            case '2':
                system("cls"); //limpa tela
                cout << "Digite a palavra no idioma novo a ser procurada: ";
                cin.getline(palavra,50); //le palavra a ser procurada
                system("cls"); //limpa tela
                p = tPesq(vocab,palavra); //funcao de busca por palavras
                if(p != NULL){
                    //mostra na tela as traducoes encontradas
                    cout << "Vocabulario: " << p->info->vocab << endl;
                    cout << "Portugues: " << p->info->PT_BR << endl;
                    cout << "Ingles: " << p->info->ING << endl;

                }else 
                    cout << "Palavra nao encontrada no vocabulario" << endl; //avisa caso a palavra nao exista no vocabulario    
                system("pause"); //pausa pra leitura
                system("cls"); //limpa tela
                menu(); //gera menu
                break;
            
            case '3':
                system("cls"); //limpa tela
                cout << "Digite a palavra a ser removida: ";
                cin.getline(palavra, 50);
                p = tPesq(vocab,palavra);
                if(p != NULL){
                    
                    tRemove(vocab, palavra);
                    cout << palavra << " Removida com sucesso" << endl;
                    write(vocab);
                
                }else 
                    cout << palavra << " Nao foi encontrado no vocabulario para ser removida" << endl;   
                system("pause"); //pausa pra leitura
                system("cls"); //limpa tela
                menu(); //gera menu
                break;
            
            case '4':
                system("cls"); // Limpa a tela
                ordenarPorTamanho(vocab); 
                system("pause"); // Pausa para leitura
                system("cls"); // Limpa a tela
                menu(); // Gera o menu
                break;
            
            case '5':
                system("cls");
                contador = contarPalavrasMesmoTamanho(vocab); 
                cout << "Quantidade de palavras no vocabulario com o mesmo tamanho que sua traducaoo em portugues: " << contador << endl;
                system("pause");
                system("cls");
                menu();
                break;
            
            case '6':
                system("cls"); 
                cout << "Digite a palavra no idioma novo a ser procurada em ingles: ";
                cin.getline(palavra,50);
                percursoRapido(vocab,palavra);
                system("pause");
                system("cls");
                menu();
                break;

            case '7':
                system("cls"); //limpa tela
                emOrdem(vocab); //lista as palavras em ordem alfabetica
                system("pause"); //pausa pra leitura
                system("cls"); //limpa tela
                menu(); //gera menu
                break;

            default:
                cout << "opcao invalida" << endl; //identifica opcao escolhida

        }

        option = getch(); //identifica opcao escolhida

    }

    cout << "numero de palavras encontradas: " << cont << endl;

    system("pause");

    delete p->info;
    delete p; //libera memoria usada em p
    tDestruir(vocab); //libera memoria da arvore   

    return 0;
}