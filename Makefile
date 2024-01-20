CC = c++
FLAGS = -Wall -Wextra -Werror -m64 -std=c++98
FLAGS_BONUS_STD = -Wall -Wextra -Werror -m64 -std=c++11

PATH_SRCS			=	src
PATH_OBJS_FT		=	obj_ft
PATH_OBJS_STD		=	obj_std
PATH_OBJS_BONUS_FT	=	obj_bonus_ft
PATH_OBJS_BONUS_STD	=	obj_bonus_std
PATH_INCLUDES		=	includes

LST_SRCS		=	main.cpp\
					utils.cpp\
					exception.cpp\
					vector_tests.cpp\
					stack_tests.cpp\
					deque_tests.cpp\
					queue_tests.cpp\
					priority_queue_tests.cpp\
					functional.cpp\
					map_tests.cpp\
					multimap_tests.cpp\
					set_tests.cpp\
					multiset_tests.cpp\
					list_tests.cpp
LST_SRCS_BONUS	=	utils.cpp\
					exception.cpp\
					functional.cpp\
					unordered_map_tests.cpp\
					unordered_set_tests.cpp\
					unordered_multimap_tests.cpp\
					unordered_multiset_tests.cpp\
					flat_basket_tests.cpp\
					main_bonus.cpp
LST_OBJS		=	${LST_SRCS:.cpp=.o}
LST_OBJS_BONUS	=	${LST_SRCS_BONUS:.cpp=.o}
LST_DEP			=	${LST_SRCS:.cpp=.d}
LST_DEP_BONUS	=	${LST_SRCS_BONUS:.cpp=.d}
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

SRC =				$(addprefix ${PATH_SRCS}/,${LST_SRCS})
SRC_BONUS =			$(addprefix ${PATH_SRCS}/,${LST_SRCS_BONUS})
DEP_FT =			$(addprefix ${PATH_OBJ_FT}/,${LST_DEP})
DEP_STD =			$(addprefix ${PATH_OBJ_STD}/,${LST_DEP})
DEP_BONUS_FT =		$(addprefix ${PATH_OBJS_BONUS_FT}/,${LST_DEP_BONUS})
DEP_BONUS_STD =		$(addprefix ${PATH_OBJS_BONUS_STD}/,${LST_DEP_BONUS})
OBJS_FT =			$(addprefix ${PATH_OBJS_FT}/,${LST_OBJS})
OBJS_STD =			$(addprefix ${PATH_OBJS_STD}/,${LST_OBJS})
OBJS_BONUS_FT =		$(addprefix ${PATH_OBJS_BONUS_FT}/,${LST_OBJS_BONUS})
OBJS_BONUS_STD =	$(addprefix ${PATH_OBJS_BONUS_STD}/,${LST_OBJS_BONUS})
INCLUDES =			$(addprefix ${PATH_INCLUDES}/,${LST_INCLUDES})

all :					AbstractDataTestsFT.exe AbstractDataTestsSTD.exe dif.exe Makefile exe

exe :
						./AbstractDataTestsFT.exe
						./AbstractDataTestsSTD.exe
						./dif.exe

AbstractDataTestsFT.exe :	${OBJS_FT} ${INCLUDES} Makefile
						${CC} ${FLAGS} ${OBJS_FT} -o $@

AbstractDataTestsSTD.exe :	${OBJS_STD} includes/test_elements.hpp Makefile 
						${CC} ${FLAGS} ${OBJS_STD} -o $@

dif.exe					:	src/diff_logs.cpp
						${CC} ${FLAGS} src/diff_logs.cpp -o $@

bonus :					AbstractDataTestsBonusFT.exe AbstractDataTestsBonusSTD.exe dif.exe Makefile bonus_exe

bonus_exe :
						./AbstractDataTestsBonusFT.exe
						./AbstractDataTestsBonusSTD.exe
						./dif.exe

AbstractDataTestsBonusFT.exe :	${OBJS_BONUS_FT} ${INCLUDES} Makefile
								${CC} ${FLAGS} ${OBJS_BONUS_FT} -o $@

AbstractDataTestsBonusSTD.exe :	${OBJS_BONUS_STD} includes/test_elements.hpp Makefile
								${CC} ${FLAGS_BONUS_STD} ${OBJS_BONUS_STD} -o $@

${PATH_OBJS_FT}/%.o:	${PATH_SRCS}/%.cpp Makefile ${INCLUDES} | ${PATH_OBJS_FT}
						${CC} ${FLAGS} -MMD -MP -DFT=1 -c $< -o $@ -I ${PATH_INCLUDES}

${PATH_OBJS_STD}/%.o:	${PATH_SRCS}/%.cpp Makefile includes/test_elements.hpp | ${PATH_OBJS_STD}
						${CC} ${FLAGS} -MMD -MP -DFT=0 -c $< -o $@ -I ${PATH_INCLUDES}

${PATH_OBJS_BONUS_FT}/%.o:	${PATH_SRCS}/%.cpp Makefile ${INCLUDES} | ${PATH_OBJS_BONUS_FT}
						${CC} ${FLAGS} -MMD -MP -DFT=1 -c $< -o $@ -I ${PATH_INCLUDES}

${PATH_OBJS_BONUS_STD}/%.o:	${PATH_SRCS}/%.cpp Makefile includes/test_elements.hpp | ${PATH_OBJS_BONUS_STD}
						${CC} ${FLAGS_BONUS_STD} -MMD -MP -DFT=0 -c $< -o $@ -I ${PATH_INCLUDES}

${PATH_OBJS_FT}:
					mkdir obj_ft

${PATH_OBJS_STD}:
					mkdir obj_std

${PATH_OBJS_BONUS_FT}:
					mkdir obj_bonus_ft

${PATH_OBJS_BONUS_STD}:
					mkdir obj_bonus_std

clean :
					rm -rf obj_ft obj_std obj_bonus_ft obj_bonus_std

fclean :			clean
					rm -f AbstractDataTestsFT.exe AbstractDataTestsSTD.exe AbstractDataTestsBonusFT.exe AbstractDataTestsBonusSTD.exe dif.exe
					rm -rf ft_logs std_logs ft_logs_bonus std_logs_bonus

re :				fclean all

-include ${DEP_FT} ${DEP_STD} ${DEP_BONUS_FT} ${DEP_BONUS_STD}

.PHONY: all re clean fclean