# PAC-MAN 👻
## Sobre o Projeto 🧠
Precisamos recriar/criar um jogo casual. Nosso grupo, pensou em um game estilo do pac-man, para o nosso trabalho da cadeira universitária PIF (Programação Imperativa e Funcional). 

## Requisitos 🧰
- Uma máquina Linux/MacOs/Windows
- Caso seja `Windows`, necessita baixar uma máquina virtual `Linux` para conseguir executar o jogo
- *__TUTORIAL__* : 
  
  [![YouTube](https://img.shields.io/badge/YouTube-%23FF0000.svg?style=for-the-badge&logo=YouTube&logoColor=white)](https://youtu.be/Gl9CCqlGOZM?si=fnIdJcxA8zisIat-)

- Baixar a Biblioteca `RAYLIB` para utilizar as funções disponibilizadas
  ``` bash
  sudo apt install libraylib-dev
  ```

- Inicializar os Compiladores na máquina virtual WSL
  ``` bash
  sudo apt install libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev build-essential cmake make g++ freeglut3-dev libglfw3 libglfw3-dev
  ```

- Comando para compilar o jogo
  ``` bash
  $ gcc *.c -o jogo -lraylib -lm -lpthread -ldl -lrt -lGL -lX11
  ```
- Comando para executar o jogo
  ``` bash
  ./jogo
  ```

## Jogo em Execução 👻
https://github.com/user-attachments/assets/e7723d0e-b824-4748-89fb-d05876ce1d31





  ## Desenvolvedores 💻
<table align="center">
  <thead>
    <tr>
      <th>Nome</th>
    </tr>
  </thead>
  <tbody>
    <tr>
    <tr>
      <td>Miguel Tojal 🖥️</td>
    </tr>
    <tr>
      <td>Mariana Maliu 🖥️</td>
    </tr>
    <tr>
      <td>Sophia Brito 🖥️</td>
    </tr>

  </tbody>
</table>
