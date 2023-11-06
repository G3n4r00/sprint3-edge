# Lixeiras Inteligentes - Longest Wave Tech

>Status: Protótipo Funcional ⚙️
>
## Visão Geral

As lixeiras inteligentes da Longest Wave Tech possuem um sistema inteligente de gerenciamento de resíduos que utiliza a tecnologia IoT (Internet das Coisas) para monitorar, alertar e melhorar o gerenciamento de lixeiras. Este sistema é projetado para melhorar a eficiência e reduzir o esforço humano envolvido na coleta e monitoramento de resíduos sólidos por meio da integração de dispositivos IoT.

## Objetivos do Projeto 🎯
O principal objetivo do projeto Clean Can da Longest Wave Tech é revolucionar a gestão de resíduos sólidos por meio de tecnologia avançada, especificamente a Internet das Coisas (IoT). O projeto visa criar um sistema inteligente de gerenciamento de lixeiras que monitora, alerta e aprimora o processo de coleta e monitoramento de resíduos.

### Objetivos Específicos 

- Eficiência na Coleta: O projeto busca melhorar a eficiência da coleta de lixo, otimizando as rotas dos caminhões de coleta e reduzindo o tempo e os recursos necessários para a coleta de resíduos.
- Redução do Desperdício de Recursos: Ao monitorar em tempo real o nível de ocupação das lixeiras, o sistema ajuda a evitar o excesso de lixo em caçambas já cheias, reduzindo o desperdício de espaço e recursos.
- Alerta de Lotação: O sistema emite alertas quando as lixeiras atingem um determinado nível de ocupação, permitindo uma resposta rápida e eficaz para evitar transbordamentos e manter as áreas limpas.
- Promoção de Cidades Inteligentes: O projeto contribui para a transformação das cidades em ambientes mais inteligentes, promovendo uma abordagem inovadora e tecnológica para a gestão urbana.

## Draft da arquitetura
![Draft da Arquitetura](img/arquitetura.png)

## Equipamentos utilizados 🧰 : 

<table> 
  <tr>
    <td>Módulo ESP32 4G</td>
    <td>Display LCD I2C</td>
    <td>Sensor Ultrassônico HC-SR04</td>
    <td>Módulo Buzzer</td>
    <td>Protoboard</td>
    <td>ESP32-CAM</td>
    <td>jumpers</td>
  </tr>
</table>

## Funcionamento 🛠️

São utilizados sensores ultrassônicos para monitorar a altura do lixo na lixeira. O sensor ultrassônico emite uma onda ultrassônica que viaja pelo ar e, quando atinge qualquer material, é refletida de volta em direção ao sensor.

Quando os dados são recolhidos e processados pelo ESP32 esses dados são anviados via MQTT ao FIWARE, de onde podemos pegar esses dados e exibi-los através de nosso dashboard, esses dados são coletados e registrados por dispositivos criados no prórprio FIWARE

### Buzzer, Display LCD 

- Quando a capacidade da lixeira atinge um determinado limite (por exemplo, 75%), o módulo Buzzer é ativado, emitindo um sinal sonoro para alertar que a lixeira está quase cheia.
- O Display LCD I2C exibe a capacidade atual da lixeira, permitindo uma visualização conveniente.


## Website e Dashboard 📊

O projeto também inclui um website dedicado. Este website contém um painel de controle com informações sobre as diferentes lixeiras, sua capacidade atual e status de ocupação. O painel exibe a localização das lixeiras em um mapa para uma melhor visualização. Os dados são atualizados regularmente e fornecem uma visão abrangente do estado das lixeiras.

## Requisitos do Sistema ⚠️

- Comunicação com a Internet para transmissão de dados.
- Utilização do Google Maps para localização e cálculo de rotas.
- Utilização de dispositivos ESP32 para controle e comunicação.
- Utilização de dashboard de terceiros para visualização de dados.
- Utilização de domínio de site na internet para acesso aos dados.
- Utilização de conexão 4G/3G para comunicação entre dispositivos.


## Instruções de Uso ❗

### Instruções de Uso para Cidadãos 👥:
- Consultar Localização e Disponibilidade das Caçambas
- Acesse o site da Longest Wave tech.
- Faça login informando seu nome de usuário e senha.
- Na aba de localização, visualize todas as caçambas de lixo mais próximas no mapa.
- Clique nas caçambas no mapa para ver a média do nível de lotação nas últimas horas.
- Se uma caçamba estiver lotada, o aplicativo ou site indicará a caçamba disponível mais próxima.

### Instruções de Uso para Departamento de Análise de Dados 📈:
- Analisar Dados Coletados
- Faça login no sistema da Longest Wave Tech.
- Acesse o dashboard para recuperar dados coletados.
- Utilize as abas do dashboard, como avisos do dia e localizações de lixeiras, para coletar os dados necessários.
- Realize cálculos e análises dos dados para identificar incidências de lotação e suas localizações.
- Categorize as informações coletadas e utilize mapas e categorias no dashboard.
- Forneça recomendações à equipe de coleta seletiva e verifique os avisos de limpeza.
- Repasse os avisos de limpeza ao departamento de limpeza da empresa.

### Instruções de Uso para Departamento de Coleta de Lixo 🚛:
- Coletar Caçambas de Lixo
- Saia da central com a rota já carregada nos dispositivos.
- Siga a rota, coletando os resíduos das caçambas de lixo ao longo do caminho.
- Marque cada caçamba coletada no sistema para acompanhamento.
- Após a coleta da última caçamba da rota, retorne à central ou siga para a próxima rota, visando a eficiência no uso de recursos.

##Equipe 
- Gabriel Genaro - RM551986
- Ricardo Akira - RM551447
- Paloma Mirela - RM551321
- Felipe Lopes - RM551464
