require 'json'

json_file_path = 'ME-25.tsl'
json_file_path_out = 'ME-25_1920.tsl'

# 読み込んで
json_data = open(json_file_path) do |io|
  JSON.load(io)
end

size = json_data['patchList'].size
for i in 0..size-1 do 
    tmp = json_data['patchList'][i]
    json_data['patchList'] << tmp
end

size = json_data['patchList'].size
for i in 0..size-1 do 
    tmp = json_data['patchList'][i]
    json_data['patchList'] << tmp
end

size = json_data['patchList'].size
for i in 0..size-1 do 
    tmp = json_data['patchList'][i]
    json_data['patchList'] << tmp
end

size = json_data['patchList'].size
for i in 0..size-1 do 
    tmp = json_data['patchList'][i]
    json_data['patchList'] << tmp
end

#json_data['patchList'][2]['orderNumber'] = 3

# 更新して
#json_data['hoge'] = 'fuga'

# 保存する
open(json_file_path_out, 'w') do |io|
  JSON.dump(json_data, io)
end
