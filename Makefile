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

CC	= gcc

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
	client_src/check_part.c \
	client_src/names.c \
	client_src/check_names.c \
	client_src/quit.c \
	client_src/users.c \
	client_src/list.c \
	client_src/check_accept.c \
	client_src/check_refuse.c \
	client_src/check_send.c \
	client_src/accept_file.c \
	client_src/send_file.c \
	client_src/refuse_file.c \
	client_src/check_quit.c \
	client_src/ident_command.c \
	client_src/check_channel_activity.c

S_SRC	= server_src/main.c \
	server_src/accept_client.c \
	server_src/command_check.c \
	server_src/join.c \
	server_src/nick.c \
	server_src/privmsg.c \
	server_src/user.c \
	server_src/arg_tools.c \
	server_src/file.c \
	server_src/list.c \
	server_src/nick_tools.c \
	server_src/quit.c \
	server_src/users.c \
	server_src/channel_tools.c \
	server_src/file_tools.c \
	server_src/loop.c \
	server_src/part.c \
	server_src/ringbuffer.c \
	server_src/welcome.c \
	server_src/client_chan_tools.c \
	server_src/free.c \
	server_src/ping.c \
	server_src/timeout.c \
	server_src/client_tools.c \
	server_src/init_server.c \
	server_src/names.c \
	server_src/pong.c \
	server_src/tools.c

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
