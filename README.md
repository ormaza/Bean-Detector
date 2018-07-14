# Bean Detector

### Descrição
Através do tratamento digital de imagem é feito a contagem da quantidade de feijões em determinada foto

### Documentação
Para o processamento da imagem foi utilizado a bibilioteca OpenCV. A imagem deve conter uma superficie branca com feijões disposta sobre ela. O algoritmo inicia sua rotina fazendo a conversão da foto para preto e branco e depois, sua transformacão em uma imagem binária. Isso é necessário para diferenciar o que é superficie e o que é feijão. Em seguida, cada feijão é rotulado usando o algoritmo FloodFill. Uma flag é incrementada a cada feijão encontrado. Depois de encontrado o feijão, a busca é expandida nas oito direções possiveis para rotular os pixels que ainda fazem parte do mesmo feijão. Finalmente, após toda a varredura da imagem, o programa retorna o numero de feijões identificados, o tamanho de cada um deles e a média de tamanho dos feijões considerando toda a amostra. Para mais detalhes ver a Seção V do paper.
