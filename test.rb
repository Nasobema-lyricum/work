require 'json'

json_file_path = 'ME-25.tsl'
json_file_path_out = 'ME-25_1920.tsl'

# ì«Ç›çûÇÒÇ≈
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

# çXêVÇµÇƒ
#json_data['hoge'] = 'fuga'

# ï€ë∂Ç∑ÇÈ
open(json_file_path_out, 'w') do |io|
  JSON.dump(json_data, io)
end
