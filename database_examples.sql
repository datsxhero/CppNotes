-- SQL 記事本資料庫查詢範例
-- 這些範例展示如何使用 SQL 查詢來管理記事

-- 1. 查看所有記事
SELECT * FROM notes ORDER BY created_at DESC;

-- 2. 查看重要記事
SELECT id, title, content, category, created_at 
FROM notes 
WHERE is_important = 1 
ORDER BY created_at DESC;

-- 3. 按分類查看記事
SELECT id, title, content, is_important, created_at 
FROM notes 
WHERE category = '工作' 
ORDER BY created_at DESC;

-- 4. 搜尋包含特定關鍵字的記事
SELECT id, title, content, category, created_at 
FROM notes 
WHERE title LIKE '%會議%' OR content LIKE '%會議%'
ORDER BY created_at DESC;

-- 5. 統計各分類的記事數量
SELECT category, COUNT(*) as count 
FROM notes 
GROUP BY category 
ORDER BY count DESC;

-- 6. 查看最近7天建立的記事
SELECT id, title, content, category, created_at 
FROM notes 
WHERE created_at >= datetime('now', '-7 days')
ORDER BY created_at DESC;

-- 7. 查看今天建立的記事
SELECT id, title, content, category, created_at 
FROM notes 
WHERE date(created_at) = date('now')
ORDER BY created_at DESC;

-- 8. 統計重要記事數量
SELECT COUNT(*) as important_count 
FROM notes 
WHERE is_important = 1;

-- 9. 查看最長的記事內容
SELECT id, title, LENGTH(content) as content_length, category 
FROM notes 
ORDER BY content_length DESC 
LIMIT 5;

-- 10. 查看每個分類的重要記事數量
SELECT category, 
       COUNT(*) as total_notes,
       SUM(is_important) as important_notes
FROM notes 
GROUP BY category 
ORDER BY total_notes DESC;

-- 11. 查看最近修改的記事
SELECT id, title, content, category, updated_at 
FROM notes 
ORDER BY updated_at DESC 
LIMIT 10;

-- 12. 搜尋特定日期的記事
SELECT id, title, content, category, created_at 
FROM notes 
WHERE date(created_at) = '2024-01-15'
ORDER BY created_at DESC;

-- 13. 查看標題長度超過20個字符的記事
SELECT id, title, content, category 
FROM notes 
WHERE LENGTH(title) > 20
ORDER BY LENGTH(title) DESC;

-- 14. 統計每月建立的記事數量
SELECT strftime('%Y-%m', created_at) as month, COUNT(*) as count 
FROM notes 
GROUP BY month 
ORDER BY month DESC;

-- 15. 查看包含多個關鍵字的記事
SELECT id, title, content, category 
FROM notes 
WHERE (title LIKE '%重要%' OR content LIKE '%重要%')
  AND (title LIKE '%會議%' OR content LIKE '%會議%')
ORDER BY created_at DESC;

-- 16. 備份重要記事到新表
CREATE TABLE IF NOT EXISTS important_notes_backup AS
SELECT * FROM notes WHERE is_important = 1;

-- 17. 清理30天前的記事（謹慎使用）
-- DELETE FROM notes WHERE created_at < datetime('now', '-30 days');

-- 18. 更新所有記事的分類（範例）
-- UPDATE notes SET category = '一般' WHERE category IS NULL OR category = '';

-- 19. 查看資料庫統計資訊
SELECT 
    (SELECT COUNT(*) FROM notes) as total_notes,
    (SELECT COUNT(*) FROM notes WHERE is_important = 1) as important_notes,
    (SELECT COUNT(DISTINCT category) FROM notes) as unique_categories,
    (SELECT MIN(created_at) FROM notes) as oldest_note,
    (SELECT MAX(created_at) FROM notes) as newest_note;

-- 20. 匯出記事到 CSV 格式（在 SQLite 命令列中使用）
-- .mode csv
-- .headers on
-- .output notes_export.csv
-- SELECT id, title, content, category, is_important, created_at, updated_at FROM notes;
-- .output stdout 