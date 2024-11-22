# **Dune II**

Bem-vindo ao **Dune II**, um jogo inspirado no épico universo de Duna. Sua missão é sobreviver aos perigos do deserto de Arrakis enquanto coleta especiarias valiosas para prosperar sua casa.

[![DuneII Trailer](https://img.youtube.com/vi/v=gd-w5tYxpn8/maxresdefault.jpg)](https://youtu.be/gd-w5tYxpn8)

---

## **Objetivo do Jogo**

- **Colete especiarias**: Espalhadas pelos mapas, elas são a chave para enriquecer sua casa e garantir sua prosperidade.
- **Venda especiarias no mercador de Arrakis**: Troque as especiarias por dinheiro e use os lucros para melhorar seu inventário e garantir sua sobrevivência.
- **Gerencie sua água**: O deserto é implacável, e sem água você não sobreviverá. Sempre que possível, compre água para não morrer.
- **Explore diferentes mapas**:
  - **Mapa 1**: Relativamente tranquilo, mas com menos especiarias.
  - **Mapa 2**: Mais perigoso, onde movimentos descuidados podem chamar a atenção dos **vermes gigantes** mais facilmente.
  - **Mapa 3**: O desafio máximo, onde sobreviver exige estratégia. Nele, você acumula muitos pontos ao coletar especiarias, mas você é facilmente identificado pelos **vermes**.

---

## **Desafios**

- **Equilibre recursos**: Colete especiarias, mas saiba conciliar com sua água, para acumular dinheiro sem morrer desidratado.
- **Evite os vermes gigantes**: Para não ser identificado por eles, não repita seus passos, e, em mapas com mais especiarias, mais fácil é de ser atacado por um deles.
- **Planeje cada movimento**: Errar pode significar a sua morte.

---

## **Como Jogar**

### **Controles**
- **WASD**: Movimente seu personagem.
- **SPACE**: Pule as cutscenes.
- **ENTER**: Confirme ações.
- **P**: Interaja com portais.
- **E**: Se mantenha parado sem respirar para evitar ser identificado pelos vermes que se rastejam sob a areia.

---

## Em Linux:

1. Se certifique que terá o GCC, make e git:
```
sudo apt install build-essential git
```

2.  Instale todas as bibliotecas necessárias:
```
sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev
```

3.  Baixe o código-fonte do Raylib diretamente do GitHub e entre na pasta:
```
git clone --depth 1 https://github.com/raysan5/raylib.git raylib
cd raylib/src/
```

4. Compile a raylib:
```
make PLATFORM=PLATFORM_DESKTOP
```

5.  Instale a raylib:
```
sudo make install
cd ~
```

6.  Clone o repositório do jogo:
```
git clone https://github.com/Mateus-Ribeir0/DuneII.git
cd DuneII
```

7.  Compile e rode o jogo:
```
make
./dune_game
```

*. Se não funcionar a compilação, tente esse comando:

```
gcc -Wall -std=c99 -Iinclude -I../raylib/src src/*.c -o dune_game -L../raylib/src -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

Caso ocorra algum imprevisto, leia a documentação da Raylib para Linux: [Raylib para Linux](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux).

<br>

## Em Mac: (O código apresenta defeitos ao compilar no Mac devido à forma que ele lida com algumas partes do nosso código, que é diferente do Linux)

1. Configure o ambiente para suportar a compilação estática:
```
export MACOSX_DEPLOYMENT_TARGET=10.9
```

2. Instale as ferramentas XCode (certifique-se de atualizar as ferramentas após a instalação):
```
xcode-select --install
```

2.1. Assegure a instalação do xcode:

```
sudo rm -rf /Library/Developer/CommandLineTools
xcode-select --install
```

3. Baixe o código-fonte do Raylib diretamente do GitHub e entre na pasta:
```
git clone https://github.com/raysan5/raylib.git
cd raylib/src
```

4. Compile a raylib:
```
make
```

5. Clone o repositório:
```
git clone https://github.com/Mateus-Ribeir0/DuneII.git
cd DuneII
```

6. Compile e rode o jogo usando o seguinte comando:
```
make
./dune_game
```

Caso ocorra algum imprevisto, leia a documentação da Raylib para Mac: [Raylib para Mac](https://github.com/raysan5/raylib/wiki/Working-on-macOS).

---
