# Raytracer

Implementação de um raytracer para a discipla de Computação Gráfica

## Como executar

Basta utilizar o comando make para compilar o executável e em seguida executá-lo com

```
bin/exe [arquivo_de_entrada] [nome_do_arquivo_de saida]
```

Esse comando executará o renderizador lendo um dado arquivo XML e criará um arquivo de imagem .ppm com o nome de saída inserido.

Exemplo de execução:

```
bin/exe example.xml example
```

## Linguagem de Descrição

O arquivo de descrição lê objetos a partir do arquivo XML fornecido e os desenha na imagem final. A seguir são descritos os tipos de objetos existentes.

### Camera

Define o tipo da câmera que será utilizada. Tem como elementos internos: Position, Target, Up, Width, Height, Vpdim, Fovy, Aspect e Fdistance. Atributos:

- type: Tipo da câmera a ser utilizada. Os valores possíveis são "orthographic" e "perspective". obrigatório.

### Position, Target e Up

Elementos internos da tag câmera que representam, respectivamente, a sua posição no mundo, o ponto central para o qual a câmera está focado, e o vetor que indica "cima", isto é, em qual direção a câmera alinhará seu topo. Os 3 objetos são pontos/vetores num plano 3D, por isso têm os seguintes atributos:

- x: coordenada x.
- y: coordenada y.
- z: coordenada z.

### Width e Height

Indicam a largura e a altura da imagem a ser gerada pela câmera, seu único atributo está a seguir:

- value: Valor da altura/largura.

### Vpdim

Indicam as dimensões a serem captadas pela câmera ortogonal, não é utilizada na câmera com perspectiva. Atributos:

- l: limite esquerdo da captura (left).
- r: limite direito da captura (right).
- b: limite inferior da captura (bottom).
- t: limite superior da captura (top).

### Fovy, Aspect, Fdistance

Indicam, respectivamente, o campo de visão vertical, o aspecto de razão da imagem, e sua distância focal. Utilizados apenas na câmera com perspectiva. Seu único atributo é o valor de cada um dos mesmos:

- value: Valor do elemento.

### Background

Define o background que será utilizado para preencher o buffer caso um raio não atinha nenhum objeto. Atributos:

- type: Define o tipo de background. Atualmente o único valor disponível é "color".

#### Funcionamento do Background

Com o tipo "color", é necessário definir até 4 cores que serão utilizada para preenchê-lo. As cores escolhidas definiram as bordas da imagem e todos os outros pontos serão preenchidos por interpolação bilinear. Dependendo do número de cores fornecido, a posição das cores dadas será diferente assim como definido a seguir:

- 1 cor: Toda o background toma a imagem da cor dada
- 2 cores: A primeira cor define o topo da imagem e a segunda a base
- 3 cores: A primeira e a segunda cor definem a região topo esquerda e topo direita respectivamente, e a terceira cor define toda a base da imagem
- 4 cores: A primeira, segunda, terceira, e quarta cores definem as regiões topo esquerda, topo direita, base direita, e base esquerda respectivamente

### Color

Define uma cor no padrão RGB. Seus atributos são:

- r: Define canal vermelho da cor.
- g: Define canal verde da cor.
- b: Define canal azul da cor.

### Integrator

Tipos de integradores. Tipos suportados atualmente: "depthMap" e "flat".

## Exemplo de Arquivo de Descrição

No arquivo de descrição a seguir é possível ver todos os tipos de objetos sendo utilizado ao mesmo tempo.

````xml
<raytracer>
	<camera type="orthographic">
		<!--- located at the origin -->
		<position x="0" y="0" z="0"/>
		<!--- looking down the -Z axis -->
		<target x="0" y="0" z="-10"/>
		<!--- the camera's up vector -->
		<up x="0" y="1" z="1"/>
		<!--- Specific parameters for perspective -->
		<!--- View plane dimensions [left right bottom top] -->
		<vpdim l="-3" r="3" b="-2.25" t="2.25" />
		<!--- Retina/image dimensions -->
		<width value="800"/>
		<height value="600"/>
	</camera>

	<scene>
		<background type="color">
			<color r="18" g="10" b="143"/>
			<!-- top left-->
			<color r="18" g="10" b="143"/>
			<!-- top right -->
			<color r="153" g="204" b="255"/>
			<!-- bottom right -->
			<color r="153" g="204" b="255"/>
			<!-- bottom left -->
		</background>
		<object type="sphere" name="sphere1" material="green">
			<radius value="0.4"/>
			<center x="-1.0" y="0.5" z="-5"/>
		</object>
		<object type="sphere" name="sphere2" material="green">
			<radius value="0.4"/>
			<center x="1.0" y="-0.5" z="-8"/>
		</object>
		<object type="sphere" name="sphere3" material="gold">
			<radius value="0.4"/>
			<center x="-1.0" y="-1.5" z="-3.5"/>
		</object>
		<object type="sphere" name="sphere4" material="pink">
			<radius value="1.0"/>
			<center x="7.0" y="1.5" z="-10"/>
		</object>

		<!-- materials -->
		<material type="flat" name="gold">
			<diffuse r="236" g="124" b="17"/>
		</material>
		<material type="flat" name="green">
			<diffuse r="42" g="201" b="51"/>
		</material>
		<material type="flat" name="pink">
			<diffuse r="253" g="153" b="253"/>
		</material>
	</scene>
	<running>		<!--- Running setup -->
		<integrator type="color">
			<spp value="1"/>
			<!--- Samples per pixel -->
		</integrator>
	</running>
</raytracer>
```
````
