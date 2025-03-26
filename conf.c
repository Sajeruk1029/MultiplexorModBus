/**
*	@file
*	@brief	Функции для работы с конфигурационным файлом. Реализация.
*	@author Кузнецов Егор Ильич EIlKuznetsov@rosatom.ru Инженер 3 категории Отдел Системной Поддержки Проектов
*	@version	1.1.0
*	@date	11.03.2024
*/

//----------------------------------------------------------------
//  Автор
//----------------------------------------------------------------
//    Кузнецов Егор Ильич
//    EIlKuznetsov@rosatom.ru
//    Инженер 3 категории
//    Отдел Системной Поддержки Проектов
//----------------------------------------------------------------
//	11.03.2024
//----------------------------------------------------------------


#include "conf.h"
	
	int8_t	createProp(const char*	name)
	{
		FILE*	file	=	0x00;

		file	=	fopen(name, "w");
	
		if(!file){ return	-1; }
		else{ fclose(file); }

		return	0;
	}
	
	int8_t	existProp(const char*	name)
	{
		FILE*	file	=	0x00;

		file	=	fopen(name, "r");

		if(file){ fclose(file); }

		return	(file != 0x00);
	}
	
	int8_t	contentProp(const char*	name, char**	buf, uint64_t	length)
	{
		FILE*	file	=	0x00;
		size_t	bytes	=	0;
	
		file	=	fopen(name, "r");
	
		if(!file){ return	-1; }
	
		*buf	=	(char*)malloc(length);
	
		if(!*buf){ return	-2; }
	
		bytes	=	fread(*buf, 1, length, file);	
		
		if(bytes != length)
		{
			free(*buf);
			*buf	=	0x00;
	
			fclose(file);
	
			return	-3;
		}
	
		fclose(file);
	
		return	0;
	}
	int8_t	contentPropOpt(FILE*	conf, char**	buf, uint64_t	length)
	{
		size_t	bytes	=	0;
	
		if(!*buf){ return	-1; }
	
		bytes	=	fread(*buf, 1, length, conf);	
		
		if(bytes != length){ return	-2; }
	
		return	0;
	}

	
	int64_t	sizeProp(const char*	name)
	{
		FILE*	file	=	0x00;

		int64_t	size	=	0;

		file	=	fopen(name, "r");

		if(!file){ return	-1; }

		fseek(file, 0, SEEK_END);

		size	=	ftell(file);

		fclose(file);
	
		return	size;
	}
	int64_t	sizePropOpt(FILE*	prop)
	{
		int64_t	size	=	0;

		if(!prop){ return	-1; }

		fseek(prop, 0, SEEK_SET);

		fseek(prop, 0, SEEK_END);

		size	=	ftell(prop);

		fseek(prop, 0, SEEK_SET);
	
		return	size;
	}


	int8_t	openFileAppend(const char*	name, FILE**	file){ return	((*file = fopen(name, "a"))? 0 : -1); }

	int8_t	closeFile(FILE*	file)
	{
		if(!file){ return	-1; }
	
		fclose(file);
	
		return	0;
	}

	int8_t	addKeyAndValueString(const char*	name, const char*	key, const char*	value)
	{
		FILE*	file	=	0x00;
	
		file	=	fopen(name, "a+");
	
		if(!file){ return	-1; }
	
		fseek(file, 0, SEEK_END);
	
		fprintf(file, "%s%c%s\n", key, '=', value);
	
		fclose(file);
	
		return	0;
	}
	int8_t	addKeyAndValueInt(const char*	name, const char*	key, int32_t	value)
	{
		FILE*	file	=	0x00;
	
		file	=	fopen(name, "a+");
	
		if(!file){ return	-1; }
	
		fseek(file, 0, SEEK_END);
	
		fprintf(file, "%s%c%i\n", key, '=', value);
	
		fclose(file);
	
		return	0;
	}
	int8_t	addKeyAndValueChar(const char*	name, const char*	key, char	value)
	{
		FILE*	file	=	0x00;
	
		file	=	fopen(name, "a+");
	
		if(!file){ return	-1; }
	
		fseek(file, 0, SEEK_END);
	
		fprintf(file, "%s%c%c\n", key, '=', value);
	
		fclose(file);
	
		return	0;
	}
	int8_t	addSection(const char*	name, const char* section)
	{
		FILE*	file	=	0x00;
	
		file	=	fopen(name, "a+");
	
		if(!file){ return	-1; }
	
		fseek(file, 0, SEEK_END);
	
		fprintf(file, "%c%s%c\n", '[', section, ']');
	
		fclose(file);
	
		return	0;
	}

	int8_t	addKeyAndValueStringOpt(FILE*	file, const char*	key, const char*	value)
	{
		if(!file){ return	-1; }
	
		fseek(file, 0, SEEK_END);
	
		fprintf(file, "%s%c%s\n", key, '=', value);
	
		return	0;
	}
	int8_t	addKeyAndValueIntOpt(FILE*	file, const char*	key, int32_t	value)
	{
		if(!file){ return	-1; }
	
		fseek(file, 0, SEEK_END);
	
		fprintf(file, "%s%c%i\n", key, '=', value);
	
		return	0;
	}
	int8_t	addKeyAndValueCharOpt(FILE*	file, const char*	key, char	value)
	{
		if(!file){ return	-1; }
	
		fseek(file, 0, SEEK_END);
	
		fprintf(file, "%s%c%c\n", key, '=', value);
	
		return	0;
	}
	int8_t	addSectionOpt(FILE*	file, const char* section)
	{
		if(!file){ return	-1; }
	
		fseek(file, 0, SEEK_END);
	
		fprintf(file, "%c%s%c\n", '[', section, ']');
	
		return	0;

	}

	int8_t	getKeys(char**	content, char***	keys, int32_t*	sizeKeys)
	{
		char*	buf	=	0x00;
		char*	pos	=	0x00;

		buf	=	strdup(*content);

		if(!buf){ return	-1; }

		pos	=	strtok(buf, "=\n");
		
		while(pos)
		{
			if(!buf){ break; }

			if(strchr(pos, '['))
			{
				pos	=	strtok(0, "=\n");

				continue;
			}

			*keys	=	(char**)	((!*sizeKeys)?	(calloc(++*sizeKeys, sizeof(char*)))	:	(realloc(*keys, ++*sizeKeys * sizeof(char*))));

			if(!*keys)
			{
				--*sizeKeys;

				free(buf);

				return	-2;
			}

			*(*keys + (*sizeKeys - 1))	=	strdup(pos);

			if(!*(*keys + (*sizeKeys - 1)))
			{
				--*sizeKeys;

				free(buf);		

				return	-3;
			}

			pos	=	strtok(0, "=\n");
			pos	=	strtok(0, "=\n");

		}

		return	0;
	}

	int8_t	getSections(char**	content, char***	sections, int32_t*	sizeSections)
	{
		char*	buf	=	0x00;
		char*	pos	=	0x00;

		buf	=	strdup(*content);

		if(!buf){ return	-1; }

		pos	=	strtok(buf, "\n");
		
		while(pos)
		{
			if(!buf){ break; }

			if(strchr(pos, '='))
			{
				pos	=	strtok(0, "\n");

				continue;
			}

			if(strchr(pos, '[') && strchr(pos, ']'))
			{
				*sections	=	(char**)	((!*sizeSections)?	(calloc(++*sizeSections, sizeof(char*)))	:	(realloc(*sections, ++*sizeSections * sizeof(char*))));

				if(!*sections)
				{
					--*sizeSections;

					free(buf);

					return	-2;
				}

				*(*sections + (*sizeSections - 1))	=	(char*)calloc(strchr(pos, ']') - strchr(pos, '[') - 1, sizeof(char));

				if(!*(*sections + (*sizeSections - 1)))
				{
					--*sizeSections;

					free(buf);		

					return	-3;
				}

				memmove(*(*sections + (*sizeSections - 1)), pos + 1, strchr(pos, ']') - strchr(pos, '[') - 1);
			}

			pos	=	strtok(0, "\n");
		}

		free(buf);

		return	0;
	}

	int8_t	getSection(char**	content, char*	section, char**	buf)
	{
		char*	buffer	=	0x00;
		char*	pos	=	0x00;
		char*	pos2	=	0x00;

		buffer	=	(char*)calloc(strlen(section) + 2, 1);

		if(!buffer){ return	-1; }

		sprintf(buffer, "%c%s%c", '[', section, ']');

		pos	=	strstr(*content, buffer);

		if(!pos){ return	-2; }

		pos	+=	strlen(section)	+	3;
		pos2	=	pos;

		pos	=	strchr(pos2, '[');

		if(!pos){ pos = strchr(pos2, '\0'); }

		//--pos;

		if(!pos)
		{
			*buf	=	strdup(pos2);

			if(!*buf)
			{
				free(buffer);

				return	-3;
			}

			free(buffer);

			return	0;
		}
		else
		{
			*buf	=	(char*)calloc(pos - pos2, sizeof(char));

			if(!*buf)
			{
				free(buffer);

				return	-3;
			}

			memmove(*buf, pos2, pos - pos2);

			free(buffer);

			return	0;
		}
	}

	int8_t	existSection(char**	content, const char*	section)
	{
		char*	buffer	=	0x00;
		char*	pos	=	0x00;

		buffer	=	(char*)calloc(strlen(section) + 2, 1);

		if(!buffer){ return	-1; }

		sprintf(buffer, "%c%s%c", '[', section, ']');

		pos	=	strstr(*content, buffer);

		free(buffer);

		return	(pos != 0x00);
	}
	int8_t	existKey(char**	content, const char*	key)
	{
		char*	buffer	=	0x00;
		char*	pos	=	0x00;

		buffer	=	(char*)calloc(strlen(key) + 1, 1);

		if(!buffer){ return	-1; }

		sprintf(buffer, "%s%c", key, '=');

		pos	=	strstr(*content, buffer);

		free(buffer);

		return	(pos != 0x00);
	}

	int8_t	getIntValue(char**	content, const char*	key, int32_t*	value)
	{
		char*	line	=	0x00;
		char*	pos	=	0x00;
		char*	buf	=	0x00;
	
		buf	=	strdup(*content);
	
		if(!buf){ return	-1; }
	
		if(!(line	=	strstr(buf, key)))
		{
			free(buf);
	
			return	-2;
		}
	
		pos	=	strtok(line, "=");
	
		if(!pos)
		{
			free(buf);
	
			return	-3;
		}
	
		pos	=	strtok(0, "=\n");
	
		if(!pos)
		{
			free(buf);
	
			return	-3;
		}

		if(!isNumber(pos))
		{
			free(buf);

			return	-4;
		}

		*value	=	atoi(pos);

		free(buf);

		buf	=	0x00;

		return	0;
	}
	
	int8_t	getCharValue(char**	content, const char*	key, char*	value)
	{
		char*	line	=	0x00;
		char*	pos	=	0x00;
		char*	buf	=	0x00;
	
		buf	=	strdup(*content);
	
		if(!buf){ return	-1; }
	
		if(!(line	=	strstr(buf, key)))
		{
			free(buf);
	
			return	-2;
		}
	
		pos	=	strtok(line, "=");
	
		if(!pos)
		{
			free(buf);
	
			return	-3;
		}
	
		pos	=	strtok(0, "=\n");
	
		if(!pos)
		{
			free(buf);
	
			return	-3;
		}
	
		*value	=	*(pos + 0);
	
		free(buf);

		buf	=	0x00;
	
		return	0;
	}
	
	int8_t	getStringValue(char**	content, const char*	key, char**	value)
	{
		char*	line	=	0x00;
		char*	pos	=	0x00;
		char*	buf	=	0x00;
	
		buf	=	strdup(*content);
	
		if(!buf){ return	-1; }
	
		if(!(line	=	strstr(buf, key)))
		{
			free(buf);
	
			return	-2;
		}
	
		pos	=	strtok(line, "=\n");
	
		if(!pos)
		{
			free(buf);
	
			return	-3;
		}
	
		pos	=	strtok(0, "=\n");
	
		if(!pos)
		{
			free(buf);
	
			return	-3;
		}
	
		*value	=	strdup(pos);
	
		if(!*value)
		{
			free(buf);
	
			return	-4;
		}
	
		free(buf);

		buf	=	0x00;

		return	0;
	}

	int8_t	isNumber(char*	line)
	{
		uint8_t	number	=	1;

		while(*line)
		{
			if(!isdigit(*line++))
			{
				number	=	0;

				break;
			}
		}

		return	number;
	}
