# Estufa com ESP32

Este projeto visa criar uma estufa utilizando a placa ESP32, integrada com diversos sensores e atuadores para monitorar e controlar diferentes parâmetros climáticos. 

Os sensores incluídos são os seguintes:

- Sensor de temperatura
- Sensor de umidade do ar
- Sensor de umidade do solo

Os atuadores incluídos são os seguintes:

- Iluminação
- Dumper
- Exaustor

O desenvolvimento deste projeto será realizado no Visual Studio Code usando a extensão PlatformIO.

## Configuração do Hardware

- ESP32
- [Sensor de temperatuda e umidade do ar (DHT11)](https://www.mouser.com/datasheet/2/758/DHT11-Technical-Data-Sheet-Translated-Version-1143054.pdf)
- [Sensor de Umidade de Solo](https://www.usinainfo.com.br/blog/sensor-de-umidade-de-solo/)

Certifique-se de conectar os sensores e atuadores corretamente ao ESP32 de acordo com as instruções do fabricante.

## Configuração do Ambiente de Desenvolvimento

1. Instale o Visual Studio Code (VSCode).
2. Instale a extensão PlatformIO no VSCode.
3. Faça o clone desse projeto.
4. Altere os campos de wifi e broker MQTT no arquivo config.h.
4. Carregue o código fonte deste repositório no projeto PlatformIO.
5. Na primera vez que o projeto for compilado, será realizado o download das bibliotecas necessárias.

## Uso

Uma vez que o hardware e o software estejam configurados corretamente, o ESP32 começará a coletar automaticamente dados dos sensores e estará pronto para receber comandos para os atuadores. Esses dados serão enviados e recebidos do broker MQTT configurado no código.

## Contribuição

Contribuições são bem-vindas! Sinta-se à vontade para fazer um fork deste repositório, trabalhar em melhorias e enviar um pull request.

## Licença

Este projeto está licenciado sob a GNU General Public License.

Para mais detalhes, consulte a [Licença](https://www.gnu.org/licenses/gpl-3.0.html).
