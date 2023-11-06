import requests
import plotly.graph_objects as go
from IPython.display import HTML, display

# Função para obter os dados de distância a partir da API
def obter_dados_distancia(lastN):
    url = f"http://46.17.108.113:8666/STH/v1/contextEntities/type/Cleancam/id/urn:ngsi-ld:Cleancam:002/attributes/level?lastN={lastN}"
    headers = {
        'fiware-service': 'smart',
        'fiware-servicepath': '/'
    }

    response = requests.get(url, headers=headers)

    if response.status_code == 200:
        data = response.json()
        distancia_data = data['contextResponses'][0]['contextElement']['attributes'][0]['values']
        return distancia_data
    else:
        print(f"Erro ao obter dados: {response.status_code}")
        return []

# Função para criar e exibir o gráfico interativo
def plotar_grafico_interativo(distancia_data):
    if not distancia_data:
        print("Nenhum dado disponível para plotar.")
        return

    distancias = [entry['attrValue'] for entry in distancia_data]
    tempos = [entry['recvTime'] for entry in distancia_data]

    # Identificar momentos de lotação
    momentos_lotacao = [(index, registro) for index, registro in enumerate(distancia_data) if registro['attrValue'] > 100]

    # Criar o gráfico interativo
    fig = go.Figure()

    # Adicionar os dados ao gráfico
    fig.add_trace(go.Scatter(x=tempos, y=distancias, mode='lines+markers', name='Distância (cm)'))
    fig.update_layout(title='Gráfico de Distância em Função do Tempo',
                      xaxis_title='Tempo',
                      yaxis_title='Distância (cm)',
                      xaxis=dict(tickangle=-45),
                      showlegend=True)

    # Adicionar marcadores para momentos de lotação
    for index, registro in momentos_lotacao:
        fig.add_trace(go.Scatter(x=[registro['recvTime']], y=[registro['attrValue']],
                                 mode='markers', marker=dict(color='red', size=10),
                                 name=f'Momento de Lotação (>100 cm) - Index {index}'))

    # Exibir o gráfico
    fig.show()

# Função para exibir o menu e obter a escolha do usuário
def obter_escolha_menu():
    print("""
    ________________      ___/-\\___     ___/-\\___     ___/-\\___
     / /             ||  |---------|   |---------|   |---------|
    / /              ||   |       |     | | | | |     |   |   |
   / /             __||   |       |     | | | | |     | | | | |
  / /   \\\\        I  ||   |       |     | | | | |     | | | | |
 (-------------------||   | | | | |     | | | | |     | | | | |
 ||               == ||   |_______|     |_______|     |_______|
 ||      L W T       | =============================================
 ||          ____    |                                ____      |
( | o      / ____ \\                                 / ____ \\    |)
 ||      / / . . \\ \\                              / / . . \\ \\   |
[ |_____| | .   . | |____________________________| | .   . | |__]
          | .   . |                                | .   . |
           \\_____/                                  \\_____/
    """)

    menu = (

        "<h2 style='color: blue;'>Bem-vindo ao Dashboard de Distância LWT!</h2>"
        "<h3 style='color: green;'>Opções:</h3>"
        "<p style='color: cyan;'>1. Visualizar dados de distância</p>"
        "<p style='color: red;'>2. Sair</p>"
    )
    display(HTML(menu))

    while True:
        escolha = input("Escolha uma opção: ")
        if escolha in ['1', '2']:
            return escolha
        else:
            print("Opção inválida. Tente novamente.")

# Função principal
def main():
    while True:
        escolha = obter_escolha_menu()
        if escolha == '1':
            lastN = int(input("Digite um valor para lastN (entre 1 e 100): "))
            distancia_data = obter_dados_distancia(lastN)
            plotar_grafico_interativo(distancia_data)
        elif escolha == '2':
            print("Saindo do programa. Até logo!")
            break

# Chamar a função principal
if __name__ == "__main__":
    main()
