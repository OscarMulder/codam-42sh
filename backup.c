
	else if (ast->type == ASSIGN)
	{
		args = create_args(ast);
		if (args == NULL)
			return ;
		builtin_assign(args, envlst, exit_code);
	}


	else if (ft_strequ(args[0], "export"))
		builtin_export(args, envlst, exit_code);