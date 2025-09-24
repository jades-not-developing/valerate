$$
\begin{align}

[\text{Prog}] &\to [\text{Stmt}]^* \\

[\text{Stmt}] &\to 
	\begin{cases}
		\text{exit}([\text{Expr}]); \\
		\text{let}\space\text{Ident} = [\text{Expr}];
	\end{cases}\\

[\text{Expr}] &\to 
	\begin{cases}
		\text{IntLit} \\
		\text{Ident}  \\
        [\text{BinExpr}] \\
	\end{cases} \\

[\text{BinExpr}] &\to 
    \begin{cases}
         [\text{Expr}] * [\text{Expr}] & \text{prec} = 1 \\
         [\text{Expr}] + [\text{Expr}] & \text{prec} = 0 \\
    \end{cases}

\end{align}
$$
