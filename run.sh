echo "Iniciando o jogo. Por favor, aguarde..."

while true; do
    ./dune_game
    if [ $? -eq 0 ]; then
        echo "Jogo executado com sucesso! Divirta-se!"
        break
    else
        echo "Houve um problema ao iniciar o jogo. Tentando novamente..."
        sleep 1
    fi
done