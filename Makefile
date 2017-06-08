##
## Makefile for 42sh in /home/VEYSSI_B/rendu/tek2/PSU/PSU_2016_myftp/
##
## Made by Baptiste veyssiere
## Login   <VEYSSI_B@epitech.net>
##
## Started on  Wed May 18 21:26:30 2016 Baptiste veyssiere
##

SERVER	= server

CLIENT	= client

RM	= rm -rf

CC	= gcc -g

MKDIR	= mkdir -p

C_SRC	= client_src/main.c \
	client_src/check_server_response.c \
	client_src/client.c \
	client_src/get_next_line.c \
	client_src/server.c \
	client_src/epur_str.c \
	client_src/strtab.c \
	client_src/check_ip_port.c \
	client_src/signal_handler.c \
	client_src/nick.c \
	client_src/write_error.c \
	client_src/check_command.c \
	client_src/join.c \
	client_src/part.c \
	client_src/message.c \
	client_src/table_manager.c \
	client_src/user_msg.c \
	client_src/channel_msg.c \
	client_src/check_waiting.c \
	client_src/pong.c \
	client_src/check_join.c \
	client_src/message_response.c \
	client_src/check_part.c

S_SRC	= server_src/main.c

C_DIR	= client_src

S_DIR	= server_src

C_ODIR	= client_obj

S_ODIR	= server_obj

C_OBJ	= $(C_SRC:$(C_DIR)/%.c=$(C_ODIR)/%.o)

S_OBJ	= $(S_SRC:$(S_DIR)/%.c=$(S_ODIR)/%.o)

CFLAGS	= -Iinclude -W -Wextra -Wall -Werror -g

$(SERVER): $(S_OBJ)
	@$(CC) -o $(SERVER) $(S_OBJ)
	@echo "Linking complete!"

$(CLIENT): $(C_OBJ)
	@$(CC) -o $(CLIENT) $(C_OBJ)
	@echo "Linking complete!"

$(C_OBJ): $(C_ODIR)/%.o : $(C_DIR)/%.c
	@$(MKDIR) $(C_ODIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

$(S_OBJ): $(S_ODIR)/%.o : $(S_DIR)/%.c
	@$(MKDIR) $(S_ODIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

all: $(SERVER) $(CLIENT)

clean:
	@$(RM) $(C_OBJ)
	@$(RM) $(S_OBJ)
	@$(RM) $(C_ODIR)
	@$(RM) $(S_ODIR)
	@echo "Cleanup complete!"

fclean: clean
	@$(RM) $(SERVER)
	@$(RM) $(CLIENT)

re: fclean all

.PHONY: all clean fclean re
