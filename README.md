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

Caso houver algum imprevisto, consulte a documentação da Raylib para Mac: [Raylib em Mac](https://github.com/raysan5/raylib/wiki/Working-on-macOS).

---
