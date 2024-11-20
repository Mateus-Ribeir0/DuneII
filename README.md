# **Dune II**

Bem-vindo ao **Dune II**, um jogo inspirado no épico universo de Duna. Sua missão é sobreviver aos perigos do deserto de Arrakis enquanto coleta especiarias valiosas para prosperar sua casa.

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

## Para instalar no Linux:

1. Baixe os essenciais:
```
sudo apt install build-essential git
```

2.  Instale as dependências:
```
 sudo apt update
 sudo apt install build-essential libgl1-mesa-dev libopenal-dev libx11-dev
```

3.  Baixe o código-fonte do Raylib diretamente do GitHub e entre na pasta:
```
git clone https://github.com/raysan5/raylib.git
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP
sudo make install
```

4.  Clone o repositório do jogo:
```
git clone https://github.com/Mateus-Ribeir0/DuneII
cd DuneII
```

5.  Compile e rode o jogo:
```
make
./dune_game
```
Caso houver algum imprevisto, consulte a documentação da Raylib para Linux: [Raylib em Linux](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux).

<br>

## Mac:

1. Instale o HomeBrew:
```
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

2. Use o Homebrew para instalar o compilador GCC e a Raylib:
```
brew install gcc
brew install raylib
```

3. Clone o repositório do jogo:
```
git clone https://github.com/Mateus-Ribeir0/DuneII
cd DuneII
```

4. Compile e rode o jogo:
```
make
```

### Sem XCode

1. No terminal:
```export MACOSX_DEPLOYMENT_TARGET=10.9```

2. Instale as ferramentas do XCode:
```xcode-select --install```

3. Builde a Raylib novamente:
```
git clone https://github.com/raysan5/raylib.git
cd raylib/src
make
```

4. Faça esse check da Otool:

```
cp raylib/src/libraylib.a YOUR_PROJECTS_ROOT_FOLDER
```

5. Clone o repositório:
```
git clone https://github.com/Mateus-Ribeir0/DuneII
```

6. Mude o diretório:
```
cd DuneII
```

7. Compile e rode:

```
make
./dune_game
```


Caso houver algum imprevisto, consulte a documentação da Raylib para Mac: [Raylib em Mac](https://github.com/raysan5/raylib/wiki/Working-on-macOS).

---
