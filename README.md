# **Dune II**

Bem-vindo ao **Dune II**, um jogo inspirado no épico universo de Duna. Sua missão é sobreviver aos perigos do deserto de Arrakis enquanto coleta especiarias valiosas para prosperar sua casa.

---

## **Objetivo do Jogo**

- **Colete especiarias**: Espalhadas pelos mapas, elas são a chave para enriquecer sua casa e garantir sua prosperidade.
- **Venda especiarias no mercador de Arrakis**: Troque as especiarias por dinheiro e use os lucros para melhorar seu inventário e garantir sua sobrevivência.
- **Gerencie sua água**: O deserto é implacável, e sem água você não sobreviverá.
- **Explore diferentes mapas**:
  - **Mapa 1**: Relativamente tranquilo, mas cuidado com as dunas.
  - **Mapa 2**: Mais perigoso, onde movimentos descuidados podem chamar a atenção dos **vermes gigantes**.
  - **Mapa 3**: O desafio máximo, onde sobreviver exige estratégia.

---

## **Desafios**

- **Equilibre recursos**: Coletar especiarias e manter sua água são tarefas que demandam planejamento.
- **Evite os vermes gigantes**: Nos mapas mais perigosos, movimentos repetitivos ou padrões atraem esses predadores mortais.
- **Planeje cada movimento**: Errar pode significar a sua morte.

---

## **Como Jogar**

### **Controles**
- **WASD**: Movimente seu personagem.
- **ENTER**: Interaja com portais ou vendedores.
- **E**: Escape de eventos perigosos, como os ataques dos vermes gigantes.

---

## **Como Configurar e Executar**

Este jogo utiliza a biblioteca gráfica **Raylib**. Veja como configurá-la no seu sistema

### **Linux**
1. Instale as dependências:
   ```bash
   sudo apt update
   sudo apt install build-essential libgl1-mesa-dev libopenal-dev libx11-dev

2. Baixe e compile a Raylib:
   
   ```git clone https://github.com/raysan5/raylib.git
   cd raylib/src
   make PLATFORM=PLATFORM_DESKTOP
   sudo make install

3. Vá para o diretório do jogo:

    ```cd DuneII```

4. Compile o jogo e rode:
   ```make
   ./dune_game


### **Mac**

1. Caso não queira buildar por você mesmo, instale o HomeBrew:

  ```/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"```

2. Assim que tiver instalado, coloque no terminal:

  ```brew install gcc```
  ```brew install raylib```
  

3. Clone o repositório:
   ```git clone https://github.com/Mateus-Ribeir0/DuneII```
   ```cd DuneII```

3. Compile o jogo:

  ```make```

4. Execute o jogo:

  ```./dune_game```

Caso tenha alguma dúvida, siga o tutorial da própria documentação da Raylib: [Tutorial da Raylib](https://github.com/raysan5/raylib/wiki/Working-on-macOS)


### **Windows**

1. Instale o MinGW:
  ```Certifique-se de instalar o pacote de compilador GCC.```

2. Baixe os arquivos pré-compilados da Raylib:
  [Binários da Raylib](https://github.com/raysan5/raylib/releases)

3. Inclua os diretórios da Raylib no MinGW:
   ```Adicione o caminho da biblioteca ao PATH ou especifique-o ao compilar```

4. Compile e Execute:
   ```gcc -o duna_game.exe main.c -lraylib -lopengl32 -lgdi32 -lwinmm
   duna_game.exe


