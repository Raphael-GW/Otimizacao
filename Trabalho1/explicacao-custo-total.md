# Como o custo total de 223 é calculado

O custo total de 223 é a soma dos custos de produção nas duas hidroelétricas com os custos de transmissão nos três arcos usados.

## Produção

A produção é dada por `Pᵢ = Fᵢ × Vᵢ`, e o custo de produção é `Cᵢ × Vᵢ`.

- **Hidroelétrica 1** produz 80: como F1 = 20, então V1 = 80/20 = 4. Custo = C1 × V1 = 2 × 4 = **8**
- **Hidroelétrica 2** produz 50: como F2 = 10, então V2 = 50/10 = 5. Custo = C2 × V2 = 1 × 5 = **5**

(Note que V1 = 4 e V2 = 5, ambos ≤ R = 10, respeitando a vazão do rio.)

## Transmissão

Os arcos definidos na entrada são:

| Arco | Capacidade | Custo |
|---|---|---|
| Hidro 1 → Central 1 | 200 | 1 |
| Hidro 2 → Central 2 | 100 | 2 |
| Central 1 → Central 2 | 100 | 1 |

O fluxo que satisfaz exatamente as demandas (D1 = 50, D2 = 80) é:

- **Hidro 1 → Central 1:** 80 unidades × custo 1 = **80**
- **Hidro 2 → Central 2:** 50 unidades × custo 2 = **100**
- **Central 1 → Central 2:** 30 unidades × custo 1 = **30**

Isso funciona porque a Central 1 recebe 80 da Hidro 1, mas só precisa de 50, então repassa o excedente (30) para a Central 2. A Central 2, por sua vez, recebe 50 da Hidro 2 + 30 da Central 1 = 80, exatamente sua demanda.

## Soma total

```
8 (produção H1) + 5 (produção H2) + 80 (H1→C1) + 100 (H2→C2) + 30 (C1→C2) = 223
```

Esse é o valor ótimo porque toda a produção é aproveitada (nada é desperdiçado) e as duas demandas são atendidas com igualdade exata, usando os arcos de menor custo disponíveis para escoar o excedente da Central 1.
