CC = c++
FLAGS = -Wall -Wextra -Werror -m64 -std=c++98

PATH_SRCS		=	src
PATH_OBJS_FT	=	obj_ft
PATH_OBJS_STD	=	obj_std
PATH_INCLUDES	=	includes

LST_SRCS		=	containers.cpp\
					exception.cpp\
					vector_tests.cpp\
					stack_tests.cpp\
					deque_tests.cpp\
					queue_tests.cpp\
					priority_queue_tests.cpp\
					functional.cpp\
					map_tests.cpp\
					utils.cpp
LST_OBJS		=	${LST_SRCS:.cpp=.o}
LST_DEP			=	${LST_SRCS:.cpp=.d}
LST_INCLUDES	=	algorithm.hpp\
					deque.hpp\
					exception.hpp\
					flat_basket.hpp\
					functional.hpp\
					iterator.hpp\
					list.hpp\
					map.hpp\
					queue.hpp\
					reverse_iterator.hpp\
					set.hpp\
					test_elements.hpp\
					treeManager.hpp\
					type_traits.hpp\
					unordered_map.hpp\
					unordered_set.hpp\
					utility.hpp\
					vector.hpp

SRC =			$(addprefix ${PATH_SRCS}/,${LST_SRCS})
DEP_FT =		$(addprefix ${PATH_OBJ_FT}/,${LST_DEP})
DEP_STD =		$(addprefix ${PATH_OBJ_STD}/,${LST_DEP})

OBJS_FT =		$(addprefix ${PATH_OBJS_FT}/,${LST_OBJS})
OBJS_STD =		$(addprefix ${PATH_OBJS_STD}/,${LST_OBJS})
INCLUDES =		$(addprefix ${PATH_INCLUDES}/,${LST_INCLUDES})

all :					AbstractDataTestsFT.exe AbstractDataTestsSTD.exe Makefile exe

exe :
						./AbstractDataTestsFT.exe
						./AbstractDataTestsSTD.exe

AbstractDataTestsFT.exe :	${OBJS_FT} ${INCLUDES} Makefile
						${CC} ${FLAGS} ${OBJS_FT} -o $@

AbstractDataTestsSTD.exe :	${OBJS_STD} Makefile 
						${CC} ${FLAGS} ${OBJS_STD} -o $@

${PATH_OBJS_FT}/%.o:	${PATH_SRCS}/%.cpp Makefile ${INCLUDES} | ${PATH_OBJS_FT}
						${CC} ${FLAGS} -MMD -MP -DFT=1 -c $< -o $@ -I ${PATH_INCLUDES}

${PATH_OBJS_STD}/%.o:	${PATH_SRCS}/%.cpp Makefile includes/test_elements.hpp | ${PATH_OBJS_STD}
						${CC} ${FLAGS} -MMD -MP -DFT=0 -c $< -o $@ -I ${PATH_INCLUDES}

${PATH_OBJS_FT}:
					mkdir obj_ft

${PATH_OBJS_STD}:
					mkdir obj_std

clean :
					rm -rf obj_ft obj_std

fclean :			clean
					rm -f AbstractDataTestsFT.exe AbstractDataTestsSTD.exe
					rm -rf ft_logs std_logs

re :				fclean all

-include ${DEP_FT} ${DEP_STD}

.PHONY: all re clean fclean